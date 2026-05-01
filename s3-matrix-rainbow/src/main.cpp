#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN     14
#define NUM_LEDS    64
#define MATRIX_W    8
#define MATRIX_H    8
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

// Convert x,y to LED index (row by row, left to right)
int xy(int x, int y) {
  if (x < 0 || x >= MATRIX_W || y < 0 || y >= MATRIX_H) return -1;
  return y * MATRIX_W + x;
}

void setPixel(int x, int y, CRGB color) {
  int idx = xy(x, y);
  if (idx >= 0) leds[idx] = color;
}

// --- Meteor config ---
#define MAX_METEORS     4     // how many simultaneous meteors
#define METEOR_LENGTH   5     // length of tail in pixels
#define METEOR_SPEED_MS 60    // lower = faster

struct Meteor {
  int     col;          // which column (0–7)
  float   pos;          // current head position (0.0 = top, 8.0 = off bottom)
  uint8_t hue;          // color
  bool    active;
};

Meteor meteors[MAX_METEORS];

void spawnMeteor(int i) {
  meteors[i].col    = random8(MATRIX_W);
  meteors[i].pos    = -METEOR_LENGTH;   // start above the screen
  meteors[i].hue    = random8(256);
  meteors[i].active = true;
}

void setup() {
  delay(1000);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)
         .setCorrection(TypicalLEDStrip);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 300);

  // Spawn meteors at staggered starting positions
  for (int i = 0; i < MAX_METEORS; i++) {
    spawnMeteor(i);
    meteors[i].pos = -random8(MATRIX_H); // stagger so they don't all start together
  }
}

void drawMeteors() {
  FastLED.clear();

  for (int m = 0; m < MAX_METEORS; m++) {
    if (!meteors[m].active) continue;

    int   head = (int)meteors[m].pos;
    uint8_t hue = meteors[m].hue;

    // Draw tail — each pixel dimmer as it goes up
    for (int t = 0; t < METEOR_LENGTH; t++) {
      int y = head - t;
      if (y < 0 || y >= MATRIX_H) continue;

      // Brightness fades from 255 (head) to ~20 (tail end)
      uint8_t brightness = 255 - (t * (255 / METEOR_LENGTH));

      // Add slight random flicker to tail for realism
      brightness = qadd8(brightness, random8(0, 20));
      brightness = qsub8(brightness, random8(0, 20));

      setPixel(meteors[m].col, y, CHSV(hue, 255, brightness));
    }

    // Advance meteor down
    meteors[m].pos += 1.0;

    // Respawn once fully off screen
    if (meteors[m].pos - METEOR_LENGTH >= MATRIX_H) {
      spawnMeteor(m);
    }
  }
}

void loop() {
  drawMeteors();
  FastLED.show();
  delay(METEOR_SPEED_MS);
}