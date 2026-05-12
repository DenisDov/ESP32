#include <Arduino.h>
#include <FastLED.h>

#define DATA_PIN               16
#define NUM_LEDS               30
#define BRIGHTNESS             50      // keep low on USB power (0-255)
#define MAX_POWER_MILLIAMPS    300
#define LED_TYPE               WS2812B
#define COLOR_ORDER            GRB     // WS2812B is GRB, not RGB

CRGB leds[NUM_LEDS];

void fadeall() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(250);
  }
}

void setup() {
  delay(500); // power-up safety delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
         .setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_POWER_MILLIAMPS);
}

void rainbowScanner() {
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

void loop() {
  rainbowScanner();
}
