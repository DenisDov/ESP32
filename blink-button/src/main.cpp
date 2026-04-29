#include <Arduino.h>

#define LED_PIN 23
#define BUTTON_PIN 22

bool ledState = false;

int lastReading = HIGH;
int buttonState = HIGH;

unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);

  // якщо змінився стан — скидаємо таймер
  if (reading != lastReading) {
    lastDebounceTime = millis();
  }

  // якщо пройшов debounce час
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // якщо стан реально змінився
    if (reading != buttonState) {
      buttonState = reading;

      // реагуємо тільки на натискання
      if (buttonState == LOW) {
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState ? HIGH : LOW);
      }
    }
  }

  lastReading = reading;
}