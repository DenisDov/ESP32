# ESP32 SSD1306 OLED Setup

## Hardware

- **Board:** ESP32-WROOM-32D (DevKit V1)
- **Display:** 0.96" I2C SSD1306 128x64 OLED (TZT)

## Wiring

| ESP32 | OLED |
|-------|------|
| 3V3 | VDD |
| GND | GND |
| GPIO21 (SDA) | SDA |
| GPIO22 (SCL) | SCK |

Some SSD1306 modules label VCC/SCL as VDD/SCK — same signals, same wiring.

## I2C address

Default is `0x3C`. Some modules ship on `0x3D` instead — if the screen stays blank after wiring is confirmed, run an I2C scanner sketch to find the actual address and update `OLED_ADDRESS` in `src/main.cpp`.
