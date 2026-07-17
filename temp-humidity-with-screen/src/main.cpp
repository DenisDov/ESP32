#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

constexpr uint8_t DHT_PIN = 4;
constexpr uint8_t DHT_TYPE = DHT22;
constexpr unsigned long READ_INTERVAL_MS = 2000;

constexpr uint8_t SCREEN_WIDTH = 128;
constexpr uint8_t SCREEN_HEIGHT = 64;
constexpr uint8_t OLED_ADDRESS = 0x3C;
constexpr int8_t OLED_RESET = -1;

// Default ESP32 I2C pins
constexpr uint8_t SDA_PIN = 21;
constexpr uint8_t SCL_PIN = 22;

DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned long lastReadAt = 0;

void setup() {
  Serial.begin(115200);
  delay(1500);

  Serial.println();
  Serial.println("DHT22 / AM2302 temperature and humidity test");
  Serial.println("Sensor data pin: GPIO 4");

  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("SSD1306 not found. Check wiring and I2C address.");
    while (true) {
      delay(1000);
    }
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Temp & Humidity");
  display.println("Waiting for sensor...");
  display.display();

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
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Sensor read failed");
    display.println("Check wiring/power");
    display.display();
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

  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Temp & Humidity");

  display.setTextSize(2);
  display.setCursor(0, 16);
  display.print(temperatureC, 1);
  display.print(" ");
  display.write(247);  // degree symbol
  display.println("C");

  display.setCursor(0, 36);
  display.print(humidity, 1);
  display.println(" %");

  display.setTextSize(1);
  display.setCursor(0, 56);
  display.print("Feels like ");
  display.print(heatIndexC, 1);
  display.print(" C");

  display.display();
}
