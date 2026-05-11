#include <FastLED.h>

#define DATA_PIN  14
#define NUM_LEDS  64

CRGB leds[NUM_LEDS];

void fadeall() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(250);
  }
}

void setup() {
  delay(2000);
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 300);
  FastLED.setBrightness(84);
}

void loop() {
  static uint8_t hue = 0;

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show();
    fadeall();
    delay(10);
  }

  for (int i = NUM_LEDS - 1; i >= 0; i--) {
    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show();
    fadeall();
    delay(10);
  }
}