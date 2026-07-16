#include <Arduino.h>
#include <DHT.h>

constexpr uint8_t DHT_PIN = 4;
constexpr uint8_t DHT_TYPE = DHT22;
constexpr unsigned long READ_INTERVAL_MS = 2000;

DHT dht(DHT_PIN, DHT_TYPE);

unsigned long lastReadAt = 0;

void setup() {
  Serial.begin(115200);
  delay(1500);

  Serial.println();
  Serial.println("DHT22 / AM2302 temperature and humidity test");
  Serial.println("Sensor data pin: GPIO 4");

  dht.begin();
}

void loop() {
  const unsigned long now = millis();
  if (now - lastReadAt < READ_INTERVAL_MS) {
    return;
  }
  lastReadAt = now;

  const float humidity = dht.readHumidity();
  const float temperatureC = dht.readTemperature();
  const float temperatureF = dht.readTemperature(true);

  if (isnan(humidity) || isnan(temperatureC) || isnan(temperatureF)) {
    Serial.println("Failed to read from DHT22 sensor. Check wiring and power.");
    return;
  }

  const float heatIndexC = dht.computeHeatIndex(temperatureC, humidity, false);
  const float heatIndexF = dht.computeHeatIndex(temperatureF, humidity);

  Serial.print("Humidity: ");
  Serial.print(humidity, 1);
  Serial.print("%  Temperature: ");
  Serial.print(temperatureC, 1);
  Serial.print(" C / ");
  Serial.print(temperatureF, 1);
  Serial.print(" F  Heat index: ");
  Serial.print(heatIndexC, 1);
  Serial.print(" C / ");
  Serial.print(heatIndexF, 1);
  Serial.println(" F");
}
