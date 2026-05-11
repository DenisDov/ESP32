#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN     14
#define NUM_LEDS    64
#define LED_TYPE    WS2812
#define COLOR_ORDER RGB
#define BRIGHTNESS  80

CRGB leds[NUM_LEDS];

void setup()
{
  delay(2000);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  FastLED.clear(true);
}

void loop()
{
  FastLED.clear();

  leds[0] = CRGB(255, 0, 0); // 1 red
  leds[1] = CRGB(0, 255, 0); // 2 green
  leds[2] = CRGB(0, 255, 0);
  leds[3] = CRGB(0, 0, 255); // 3 blue
  leds[4] = CRGB(0, 0, 255);
  leds[5] = CRGB(0, 0, 255);

  FastLED.show();
  delay(1000);
}
