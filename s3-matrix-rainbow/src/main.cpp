#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN     14 // Default data pin for the Waveshare ESP32-S3-Matrix
#define NUM_LEDS    64 // 8x8 Matrix
#define BRIGHTNESS  50 // Keep this low (0-255) to avoid drawing too much power over USB
#define LED_TYPE    WS2812
#define COLOR_ORDER RGB // Change to GRB if the rainbow colors look scrambled

CRGB leds[NUM_LEDS];
uint8_t startIndex = 0;

void setup() {
  delay(1000); // Safety power-up delay to let the ESP32-S3 stabilize
  
  // Initialize the matrix
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  // fill_rainbow(led array, number of LEDs, starting hue, hue change per LED)
  fill_rainbow(leds, NUM_LEDS, startIndex, 7); 
  
  FastLED.show();
  FastLED.delay(20); // Controls the frame rate/speed of the animation
  
  startIndex++; // Shifts the starting color slightly every frame to animate the rainbow
}