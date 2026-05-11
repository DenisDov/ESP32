#include <FastLED.h>

#define DATA_PIN  14
#define NUM_LEDS  64

CRGB leds[NUM_LEDS];

void setup() {
  delay(2000);
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 300);
}

void loop() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::White;
    FastLED.show();
    delay(100);
    leds[i] = CRGB::Black;
  }
}