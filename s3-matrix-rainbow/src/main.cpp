#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN     14
#define NUM_LEDS    64
#define MATRIX_W    8
#define MATRIX_H    8
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

int xy(int x, int y) {
  return y * MATRIX_W + x;
}

// --- XOR config ---
#define TIME_SCALE   3      // how fast the pattern animates (higher = faster)
#define COLOR_SCALE  8      // how many colors spread across the grid (higher = more)
#define SATURATION   240    // 255 = fully saturated, lower = more pastel

uint16_t t = 0;

// --- Variants --- (swap which drawXOR you call in loop)

// Classic XOR — sharp geometric interference pattern
void drawXOR() {
  for (int y = 0; y < MATRIX_H; y++) {
    for (int x = 0; x < MATRIX_W; x++) {
      uint8_t hue = ((x ^ y) * COLOR_SCALE) + t;
      leds[xy(x, y)] = CHSV(hue, SATURATION, 255);
    }
  }
}

// Animated XOR with sine-warped coordinates — much more organic
void drawXORWarped() {
  for (int y = 0; y < MATRIX_H; y++) {
    for (int x = 0; x < MATRIX_W; x++) {
      // Warp x and y with sine waves before XOR-ing
      uint8_t wx = x + (sin8(y * 30 + t) >> 5);     // shift x by ~0–7
      uint8_t wy = y + (sin8(x * 30 + t / 2) >> 5); // shift y by ~0–7

      uint8_t hue = ((wx ^ wy) * COLOR_SCALE) + t;
      leds[xy(x, y)] = CHSV(hue, SATURATION, 255);
    }
  }
}

// XOR combined with OR — creates diamond/moiré interference
void drawXORMoire() {
  for (int y = 0; y < MATRIX_H; y++) {
    for (int x = 0; x < MATRIX_W; x++) {
      uint8_t xorVal = (x ^ y) * COLOR_SCALE;
      uint8_t orVal  = (x | y) * COLOR_SCALE;
      uint8_t hue    = xorVal + orVal + t;
      uint8_t bri    = sin8(xorVal + t);   // brightness also pulses
      leds[xy(x, y)] = CHSV(hue, SATURATION, bri);
    }
  }
}

// --- Variant auto-cycling ---
enum Variant { CLASSIC, WARPED, MOIRE, NUM_VARIANTS };
Variant current = CLASSIC;
unsigned long lastSwitch = 0;

void setup() {
  delay(1000);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)
         .setCorrection(TypicalLEDStrip);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 300);
}

void loop() {
  // Cycle variants every 6 seconds
  if (millis() - lastSwitch > 6000) {
    lastSwitch = millis();
    current    = (Variant)((current + 1) % NUM_VARIANTS);
  }

  switch (current) {
    case CLASSIC: drawXOR();       break;
    case WARPED:  drawXORWarped(); break;
    case MOIRE:   drawXORMoire();  break;
    default: break;
  }

  FastLED.show();
  t += TIME_SCALE;
  delay(16);
}