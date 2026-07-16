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

Default is `0x3C`. Some modules ship on `0x3D` instead — if the screen stays blank, use the I2C scanner below to find the actual address and update `OLED_ADDRESS` in `src/main.cpp`.

## Troubleshooting: blank screen

Copy `variants/i2c_scanner.cpp` into `src/main.cpp`, flash, and check Serial Monitor (115200 baud):

- **Address found (e.g. `0x3C`)** — bus is fine, just update `OLED_ADDRESS` if it's not `0x3C`. Copy the real sketch back into `main.cpp`.
- **No devices found** — almost always SDA/SCK swapped or a loose breadboard connection, not a power issue. Reseat all 4 wires and double check SDA↔GPIO21 and SCK↔GPIO22 aren't crossed.
