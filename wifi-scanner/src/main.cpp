#include "WiFi.h"  // Бібліотека для роботи з Wi-Fi

void setup() {
  Serial.begin(115200);  // Ініціалізація серійного монітора
  delay(1000);           // Коротка затримка для стабілізації
  Serial.println("WiFi Scanner");

  // Ініціалізація Wi-Fi у режимі клієнта
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();    // Відключаємось від усіх мереж перед скануванням
  delay(100);           // Коротка затримка перед початком сканування
}

void loop() {
  Serial.println("Scanning for networks...");

  // Виконання сканування доступних мереж
  int networkCount = WiFi.scanNetworks();

  if (networkCount == 0) {
    Serial.println("No networks found");
  } else {
    Serial.printf("%d networks found:\n", networkCount);

    // Виведення інформації про кожну мережу
    for (int i = 0; i < networkCount; ++i) {
      Serial.printf("%d: %s, Signal: %d dBm, Encryption: %s\n", 
                    i + 1, 
                    WiFi.SSID(i).c_str(), 
                    WiFi.RSSI(i),
                    (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "Open" : "Encrypted");
      delay(10);  // Невелика затримка для серійного виведення
    }
  }

  Serial.println("");  // Додаємо пустий рядок для форматування
  delay(5000);  // Чекаємо 5 секунд перед наступним скануванням
}