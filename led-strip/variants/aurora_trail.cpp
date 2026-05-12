#include <Arduino.h>
#include <FastLED.h>

#define DATA_PIN    16
#define NUM_LEDS    30
#define BRIGHTNESS  50      // keep low on USB power (0–255)
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB     // WS2812B is GRB, not RGB

CRGB leds[NUM_LEDS];

DEFINE_GRADIENT_PALETTE(aurora_gp) {
    0,   0,   6,  18,
   45,   0,  90, 100,
  110,   0, 210, 150,
  170, 130,  40, 210,
  220,  20,   0,  80,
  255,   0,   6,  18
};

CRGBPalette16 auroraPalette = aurora_gp;

void setup() {
  delay(500); // power-up safety delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
         .setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}

void auroraTrail() {
  static uint8_t drift = 0;
  const uint8_t shimmer = beatsin8(7, 20, 95);

  for (int i = 0; i < NUM_LEDS; i++) {
    uint8_t wave = sin8((i * 18) + drift);
    uint8_t colorIndex = wave + (i * 5);
    uint8_t brightness = qadd8(scale8(wave, 150), shimmer);

    leds[i] = ColorFromPalette(auroraPalette, colorIndex, brightness, LINEARBLEND);
  }

  uint8_t comet = beatsin8(13, 0, NUM_LEDS - 1);
  leds[comet] += CHSV(drift + 96, 90, 160);

  if (random8() < 18) {
    leds[random8(NUM_LEDS)] += CRGB(80, 80, 120);
  }

  FastLED.show();
  fadeToBlackBy(leds, NUM_LEDS, 18);
  drift++;
  delay(24);
}

void loop() {
  auroraTrail();
}
