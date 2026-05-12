# ESP32 WS2812B Basic Setup

## Hardware

- **Board:** ESP32-WROOM-32D (DevKit V1)
- **Strip:** WS2812B 1M, 30 LEDs, IP30

## Wiring

| ESP32 | Wire | Strip |
|-------|------|-------|
| VIN | Red | +5V |
| GND | White | GND |
| RX2 (GPIO16) | Green → 220Ω → | DIN |

## FastLED config

```cpp
#define DATA_PIN    16
#define NUM_LEDS    30
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
```

## Simulation

Wokwi diagram available in `diagram.json`.

## Variants

Example effects are kept in `/variants`. Copy one into `main.cpp`, then build and flash.
