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
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 300);
}

void whiteChase() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::White;
    FastLED.show();
    delay(100);
    leds[i] = CRGB::Black;
  }
}

void loop() {
  whiteChase();
}
