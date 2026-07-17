# ESP32 Playground

A collection of small, self-contained ESP32 projects built with [PlatformIO](https://platformio.org/) and the Arduino framework. Each folder is an independent PlatformIO project — open it directly in VS Code (with the PlatformIO extension) or build it from the command line.

Most projects target the classic **ESP32 DevKit** (`esp32dev`); `s3-matrix-rainbow` targets the **ESP32-S3 DevKitC-1**.

## Projects

| Project | Description | Hardware / Libraries |
|---|---|---|
| [blink](blink/) | Classic blink using the onboard LED (GPIO 2) | — |
| [blink-with-resistor](blink-with-resistor/) | Blink with an external LED + resistor on GPIO 23 | External LED, resistor |
| [blink-button](blink-button/) | Toggle an LED with a push button, with software debouncing (GPIO 23 / 22) | LED, push button |
| [wifi-scanner](wifi-scanner/) | Scans nearby Wi-Fi networks and prints them to the serial monitor | — |
| [ble-beacon](ble-beacon/) | BLE server broadcasting periodic iBeacon frames | — |
| [keto-gluco-buttons](keto-gluco-buttons/) | Two-button BLE device that sends glucose/ketone measurement events as JSON over BLE | Push buttons ×2, ArduinoJson |
| [temp-humidity](temp-humidity/) | Reads a DHT22 (AM2302) sensor and logs temperature/humidity to serial | DHT22, Adafruit DHT + Unified Sensor |
| [temp-humidity-with-screen](temp-humidity-with-screen/) | DHT22 readings, intended for display on a screen | DHT22, Adafruit DHT + Unified Sensor |
| [lcd-screen](lcd-screen/) | Drives a 128×64 SSD1306 OLED over I2C (SDA 21 / SCL 22, address 0x3C) | SSD1306 OLED, Adafruit SSD1306 + GFX |
| [soil-moisture](soil-moisture/) | Capacitive soil-moisture sensor on GPIO 34 with dry/wet calibration and watering alerts | Soil-moisture sensor |
| [led-strip](led-strip/) | WS2812B strip animations (30 LEDs on GPIO 16) with extra effect variants in `variants/` | WS2812B strip, FastLED |
| [s3-matrix-rainbow](s3-matrix-rainbow/) | Color-palette animations on an 8×8 WS2812 matrix (GPIO 14), ESP32-S3 | 8×8 LED matrix, FastLED |

Some projects have their own README with wiring diagrams and troubleshooting notes — see [led-strip/README.md](led-strip/README.md) and [s3-matrix-rainbow/README.md](s3-matrix-rainbow/README.md).

## Getting started

1. Install [VS Code](https://code.visualstudio.com/) with the [PlatformIO IDE extension](https://platformio.org/install/ide?install=vscode), or the [PlatformIO CLI](https://docs.platformio.org/en/latest/core/installation/index.html).
2. Connect your ESP32 board over USB.
3. Build and upload a project:

```sh
cd blink            # or any other project folder
pio run -t upload   # build and flash
pio device monitor  # open the serial monitor (most projects use 115200 baud)
```

PlatformIO downloads the correct toolchain and library dependencies (declared in each project's `platformio.ini`) automatically on first build.

## Notes

- Serial monitor baud rate is **115200** for projects that print output.
- Pin assignments are defined at the top of each project's `src/main.cpp` — adjust them to match your wiring.
- If upload fails, hold the **BOOT** button on the board while PlatformIO connects.
