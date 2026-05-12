#include <Arduino.h>
#include <FastLED.h>

#define DATA_PIN    16
#define NUM_LEDS    30
#define BRIGHTNESS  50      // keep low on USB power (0-255)
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB     // WS2812B is GRB, not RGB

CRGB leds[NUM_LEDS];

void setup() {
  delay(500); // power-up safety delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
         .setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}

void emberComet() {
  fadeToBlackBy(leds, NUM_LEDS, 34);

  uint8_t head = beatsin8(18, 0, NUM_LEDS - 1);
  uint8_t heat = beatsin8(9, 160, 255);

  leds[head] += HeatColor(heat);

  if (head > 0) {
    leds[head - 1] += CRGB(90, 18, 0);
  }

  if (head < NUM_LEDS - 1) {
    leds[head + 1] += CRGB(90, 18, 0);
  }

  if (random8() < 45) {
    uint8_t ember = random8(NUM_LEDS);
    leds[ember] += CRGB(random8(40, 130), random8(4, 24), 0);
  }

  EVERY_N_MILLISECONDS(120) {
    blur1d(leds, NUM_LEDS, 48);
  }

  FastLED.show();
  delay(22);
}

void loop() {
  emberComet();
}
