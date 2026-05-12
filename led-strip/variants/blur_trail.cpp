#include <Arduino.h>
#include <FastLED.h>

#define DATA_PIN               16
#define NUM_LEDS               30
#define BRIGHTNESS             50      // keep low on USB power (0-255)
#define MAX_POWER_MILLIAMPS    300
#define LED_TYPE               WS2812B
#define COLOR_ORDER            GRB     // WS2812B is GRB, not RGB

CRGB leds[NUM_LEDS];

uint8_t pos = 0;
bool toggle = false;

void setup() {
  Serial.begin(115200);
  delay(500); // power-up safety delay

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
         .setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_POWER_MILLIAMPS);
  FastLED.clear(true);

  Serial.println("Blur trail example started");
}

void blurTrail() {
  leds[pos] = CHSV(pos * 8, 255, 255);

  blur1d(leds, NUM_LEDS, 172);
  fadeToBlackBy(leds, NUM_LEDS, 16);

  FastLED.show();

  if (toggle) {
    pos = (pos + 1) % NUM_LEDS;
  }

  toggle = !toggle;

  delay(20);
}

void loop() {
  blurTrail();
}
