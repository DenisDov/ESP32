# AGENTS.md

Guidance for coding agents working in this repository.

## Project Overview

This is a small PlatformIO project for an ESP32 DevKit running Arduino framework code to drive a WS2812B LED strip with FastLED.

- Target environment: `esp32dev`
- Framework: Arduino
- Main firmware: `src/main.cpp`
- Hardware notes and Wokwi diagram: `docs/`
- Current LED setup: 30 WS2812B LEDs on GPIO16, GRB color order

## Repository Layout

- `platformio.ini` contains the board, framework, and library dependencies.
- `src/` contains firmware source files.
- `include/` is for shared project headers.
- `lib/` is for private project libraries.
- `test/` is for PlatformIO tests.
- `docs/` contains wiring notes and simulation assets.
- `variants/` contains simple effect examples that can be copied into `src/main.cpp` and flashed.

## Common Commands

Run commands from the repository root.

```sh
pio run
pio run -t upload
pio device monitor
pio test
```

If `pio` is not available in the shell, do not rewrite the project around another build system. Ask the user or note the missing local dependency.

## Hardware Assumptions

Preserve these defaults unless the user asks to change the hardware configuration:

- Board: ESP32-WROOM-32D / DevKit V1 style board
- LED strip: WS2812B, 30 LEDs
- Data pin: GPIO16 / RX2
- Color order: `GRB`
- Brightness: keep conservative for USB power

Be careful with changes that increase current draw. Large brightness or all-white effects may exceed what USB power can safely provide.

## Coding Conventions

- Keep firmware changes simple and readable; this project is intentionally small.
- Prefer Arduino/FastLED idioms already present in `src/main.cpp`.
- Prefer keeping standalone effect examples in `variants/` instead of adding complex mode-switching infrastructure.
- When creating or improving effects, consider adapting examples from the official FastLED examples collection: https://github.com/FastLED/FastLED/tree/master/examples
- Put reusable declarations in `include/` only when they are shared across multiple source files.
- Add comments only where they clarify hardware constraints, timing, or non-obvious behavior.
- Keep generated artifacts out of the repo unless the user explicitly asks for them.

## Verification

For code changes, prefer at least:

```sh
pio run
```

For upload or monitor requests, use the PlatformIO upload and monitor commands above. If physical hardware is not connected, report that clearly instead of treating upload failure as a firmware failure.

<!-- When changing wiring, pin numbers, LED count, or color order, update `docs/README.md` and `docs/diagram.json` as needed so the documentation stays aligned with the firmware. -->
