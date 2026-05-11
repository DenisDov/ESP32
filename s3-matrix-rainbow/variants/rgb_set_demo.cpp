#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN     14
#define NUM_LEDS    64
#define LED_TYPE    WS2812
#define COLOR_ORDER RGB
#define BRIGHTNESS  80

CRGBArray<NUM_LEDS> leds;

void setup()
{
  delay(2000);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear(true);
}

void loop()
{
  static uint8_t hue;

  for (int i = 0; i < NUM_LEDS / 2; i++)
  {
    leds.fadeToBlackBy(40);

    leds[i] = CHSV(hue++, 255, 255);

    leds(NUM_LEDS / 2, NUM_LEDS - 1) = leds(NUM_LEDS / 2 - 1, 0);

    FastLED.delay(33);
  }
}
