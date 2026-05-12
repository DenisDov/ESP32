# DHT22 / AM2302 ESP32 Wiring

This sketch reads a DHT22 / AM2302 temperature and humidity module from GPIO 4.

## Schema

```text
ESP32 DevKit                 DHT22 / AM2302 module
---------------------------------------------------
3V3        ----------------> VCC / +
GPIO 4     ----------------> DATA / OUT / S
GND        ----------------> GND / -
```

## Notes

- Power the module from `3V3`, not `5V`, so the data signal is safe for the ESP32.
- Most AM2302 modules already include the needed pull-up resistor on the data line.
- If you have a bare 4-pin DHT22 sensor instead of a module, add a 4.7k-10k resistor between `VCC` and `DATA`.
- The sketch prints readings every 2 seconds at `115200` baud.

## Run

```sh
pio run
pio run --target upload
pio device monitor
```
