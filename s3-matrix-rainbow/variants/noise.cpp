#include <FastLED.h>

#define DATA_PIN    14
#define LED_TYPE    WS2812
#define COLOR_ORDER RGB
#define BRIGHTNESS  96

const uint8_t kMatrixWidth  = 8;
const uint8_t kMatrixHeight = 8;
const bool    kMatrixSerpentineLayout = true;

#define MAX_DIMENSION 8
#define NUM_LEDS      (kMatrixWidth * kMatrixHeight)

uint16_t speed = 20;  // try 1 (painting) … 100 (water)
uint16_t scale = 311; // try 4011 for very zoomed out

CRGB     leds[NUM_LEDS];
uint8_t  noise[MAX_DIMENSION][MAX_DIMENSION];

static uint16_t x, y, z;

uint16_t XY(uint8_t x, uint8_t y) {
  if (kMatrixSerpentineLayout && (y & 1)) {
    return y * kMatrixWidth + (kMatrixWidth - 1 - x);
  }
  return y * kMatrixWidth + x;
}

void fillnoise8() {
  for (int i = 0; i < MAX_DIMENSION; i++) {
    int ioffset = scale * i;
    for (int j = 0; j < MAX_DIMENSION; j++) {
      noise[i][j] = inoise8(x + ioffset, y + scale * j, z);
    }
  }
  z += speed;
}

void setup() {
  delay(3000);
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 300);
  FastLED.setBrightness(BRIGHTNESS);

  x = random16();
  y = random16();
  z = random16();
}

void loop() {
  fillnoise8();

  for (int i = 0; i < kMatrixWidth; i++) {
    for (int j = 0; j < kMatrixHeight; j++) {
      // hue from noise[j][i], brightness from noise[i][j]
      leds[XY(i, j)] = CHSV(noise[j][i], 255, noise[i][j]);
    }
  }

  FastLED.show();
}