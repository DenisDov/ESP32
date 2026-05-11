#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN     14
#define NUM_LEDS    64
#define LED_TYPE    WS2812
#define COLOR_ORDER RGB
#define BRIGHTNESS  80

#define WIDTH       8
#define HEIGHT      8

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
  for (int i = 0; i < NUM_LEDS; i++)
  {
    int row = i / WIDTH;

    if (row < HEIGHT / 2)
    {
      leds[i] = CRGB::Blue;
    }
    else
    {
      leds[i] = CRGB::Yellow;
    }
  }

  FastLED.show();
  delay(1000);
}
