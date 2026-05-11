#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN   14
#define NUM_LEDS  64
#define LED_TYPE  WS2812
#define COLOR_ORDER RGB

CRGB leds[NUM_LEDS];

void setup() {
  delay(1000);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 300);
}

void loop() {
  // All LEDs red
  fill_solid(leds, NUM_LEDS, CRGB::Red);
  FastLED.show();
  delay(500);

  // All LEDs off
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  delay(100);
}