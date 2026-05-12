#include <Arduino.h>
#include <FastLED.h>

#define DATA_PIN    16
#define NUM_LEDS    30
#define BRIGHTNESS  50      // keep low on USB power (0–255)
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB     // WS2812B is GRB, not RGB

CRGB leds[NUM_LEDS];

void setup() {
  delay(500); // power-up safety delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
         .setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}

void rainbow() {
  static uint8_t hue = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue + (i * 255 / NUM_LEDS), 255, 255);
  }
  FastLED.show();
  hue++;
  delay(20);
}

void loop() {
  rainbow();
}

