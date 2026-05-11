#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN          14
#define NUM_LEDS         64
#define MATRIX_W         8
#define MATRIX_H         8
#define LED_TYPE         WS2812
#define COLOR_ORDER      GRB
#define FRAMES_PER_SECOND 120

CRGB leds[NUM_LEDS];

uint8_t gHue = 0; // rotating base color

// ----------------------------------------------------------------
// Effect 1: smooth rainbow across all LEDs
// ----------------------------------------------------------------
void rainbow() {
  fill_rainbow(leds, NUM_LEDS, gHue, 7);
}

// ----------------------------------------------------------------
// Effect 2: rainbow + random sparkle glitter
// ----------------------------------------------------------------
void addGlitter(fract8 chanceOfGlitter) {
  if (random8() < chanceOfGlitter) {
    leds[random16(NUM_LEDS)] += CRGB::White;
  }
}

void rainbowWithGlitter() {
  rainbow();
  addGlitter(80);
}

// ----------------------------------------------------------------
// Effect 3: random colored speckles that blink in and fade smoothly
// ----------------------------------------------------------------
void confetti() {
  fadeToBlackBy(leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV(gHue + random8(64), 200, 255);
}

// ----------------------------------------------------------------
// Effect 4: a colored dot sweeping back and forth with fading trail
// ----------------------------------------------------------------
void sinelon() {
  fadeToBlackBy(leds, NUM_LEDS, 20);
  int pos = beatsin16(13, 0, NUM_LEDS - 1);
  leds[pos] += CHSV(gHue, 255, 192);
}

// ----------------------------------------------------------------
// Effect 5: colored stripes pulsing at a defined BPM
// ----------------------------------------------------------------
void bpm() {
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}

// ----------------------------------------------------------------
// Effect 6: eight colored dots, weaving in and out of sync
// ----------------------------------------------------------------
void juggle() {
  fadeToBlackBy(leds, NUM_LEDS, 20);
  uint8_t dothue = 0;
  for (int i = 0; i < 8; i++) {
    leds[beatsin16(i + 7, 0, NUM_LEDS - 1)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

// ----------------------------------------------------------------
// Effect list - add/remove/reorder freely
// ----------------------------------------------------------------
typedef void (*EffectFunc)();
EffectFunc effects[] = {
  rainbow,
  rainbowWithGlitter,
  confetti,
  sinelon,
  juggle,
  bpm,
};
const uint8_t NUM_EFFECTS = sizeof(effects) / sizeof(effects[0]);
uint8_t currentEffect = 0;

void setup() {
  delay(1000);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)
         .setCorrection(TypicalLEDStrip);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 300);
}

void loop() {
  // Auto-advance effect every 10 seconds
  EVERY_N_SECONDS(10) {
    currentEffect = (currentEffect + 1) % NUM_EFFECTS;
  }

  // Slowly cycle the base color
  EVERY_N_MILLISECONDS(20) {
    gHue++;
  }

  effects[currentEffect]();

  FastLED.show();
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}