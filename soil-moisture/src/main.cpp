#include <Arduino.h>

const int SENSOR_PIN = 34;

// Calibrate these for your sensor:
const int AIR_VALUE   = 3200;  // sensor reading in open air (dry)
const int WATER_VALUE = 1400;  // sensor reading submerged in water (wet)

void setup() {
  Serial.begin(115200);
  analogReadResolution(12); // ESP32 default: 12-bit (0–4095)
}

void loop() {
  int raw = analogRead(SENSOR_PIN);

  // Map to 0–100% moisture
  int moisture = map(raw, AIR_VALUE, WATER_VALUE, 0, 100);
  moisture = constrain(moisture, 0, 100);

  Serial.printf("Raw: %d | Moisture: %d%%\n", raw, moisture);

  if (moisture < 25) {
    Serial.println("⚠ Rosemary needs water!");
  } else if (moisture > 55) {
    Serial.println("⚠ Too wet — risk of root rot!");
  } else {
    Serial.println("✓ Soil moisture looks good");
  }

  delay(2000);
}