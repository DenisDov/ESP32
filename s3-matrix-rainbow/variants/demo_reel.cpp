#include <FastLED.h>

#define DATA_PIN      14
#define LED_TYPE      WS2812
#define COLOR_ORDER   RGB
#define NUM_LEDS      64
#define BRIGHTNESS    96
#define FRAMES_PER_SECOND 120

CRGB leds[NUM_LEDS];

void setup() {
  delay(3000);
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 300);
  FastLED.setBrightness(BRIGHTNESS);
}

void rainbow();
void rainbowWithGlitter();
void confetti();
void sinelon();
void bpm();
void juggle();
void nextPattern();

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };

uint8_t gCurrentPatternNumber = 0;
uint8_t gHue = 0;

void loop() {
  gPatterns[gCurrentPatternNumber]();
  FastLED.show();
  FastLED.delay(1000 / FRAMES_PER_SECOND);
  EVERY_N_MILLISECONDS(20) { gHue++; }
  EVERY_N_SECONDS(10)      { nextPattern(); }
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern() {
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
}

void rainbow() {
  fill_rainbow(leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() {
  rainbow();
  if (random8() < 80) leds[random16(NUM_LEDS)] += CRGB::White;
}

void confetti() {
  fadeToBlackBy(leds, NUM_LEDS, 10);
  leds[random16(NUM_LEDS)] += CHSV(gHue + random8(64), 200, 255);
}

void sinelon() {
  fadeToBlackBy(leds, NUM_LEDS, 20);
  leds[beatsin16(13, 0, NUM_LEDS - 1)] += CHSV(gHue, 255, 192);
}

void bpm() {
  uint8_t beat = beatsin8(62, 64, 255);
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = ColorFromPalette(PartyColors_p, gHue + (i * 2), beat - gHue + (i * 10));
}

void juggle() {
  fadeToBlackBy(leds, NUM_LEDS, 20);
  uint8_t dothue = 0;
  for (int i = 0; i < 8; i++) {
    leds[beatsin16(i + 7, 0, NUM_LEDS - 1)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}