#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN     14
#define NUM_LEDS    64
#define LED_TYPE    WS2812
#define COLOR_ORDER RGB
#define BRIGHTNESS  128

CRGB leds[NUM_LEDS];

uint8_t pos = 0;
bool toggle = false;

void setup()
{
  Serial.begin(115200);
  delay(2000);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear(true);

  Serial.println("Blur example started");
}

void loop()
{
  leds[pos] = CHSV(pos * 4, 255, 255);

  blur1d(leds, NUM_LEDS, 172);
  fadeToBlackBy(leds, NUM_LEDS, 16);

  FastLED.show();

  if (toggle)
  {
    pos = (pos + 1) % NUM_LEDS;
  }

  toggle = !toggle;

  delay(20);
}
