#include <Arduino.h>
#include <FastLED.h>

#define DATA_PIN               16
#define NUM_LEDS               30
#define BRIGHTNESS             50      // keep low on USB power (0-255)
#define MAX_POWER_MILLIAMPS    300
#define LED_TYPE               WS2812B
#define COLOR_ORDER            GRB     // WS2812B is GRB, not RGB

uint16_t speed = 20;   // try 1 for slow painting, 100 for faster water
uint16_t scale = 311;  // higher values zoom the noise pattern out

CRGB leds[NUM_LEDS];

static uint16_t x;
static uint16_t z;

void setup() {
  delay(500); // power-up safety delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
         .setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_POWER_MILLIAMPS);

  x = random16();
  z = random16();
}

void noiseFlow() {
  for (int i = 0; i < NUM_LEDS; i++) {
    uint16_t offset = scale * i;
    uint8_t hue = inoise8(x + offset, z);
    uint8_t brightness = inoise8(x + offset + 10000, z + 30000);

    leds[i] = CHSV(hue, 255, qadd8(brightness, 35));
  }

  z += speed;
  FastLED.show();
  delay(20);
}

void loop() {
  noiseFlow();
}
