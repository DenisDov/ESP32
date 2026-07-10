#include "Arduino.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <ArduinoJson.h>

// ---------- Pins ----------
#define LED_PIN         2
#define BTN_GLUCOSE_PIN 4   // button leg -> GPIO4, other leg -> GND (INPUT_PULLUP)
#define BTN_KETONE_PIN  5   // button leg -> GPIO5, other leg -> GND (INPUT_PULLUP)

// ---------- BLE ----------
#define SERVICE_UUID  "12345678-1234-1234-1234-123456789abc"
#define EVENTS_CHAR   "12345678-1234-1234-1234-123456789001"
#define CONTROL_CHAR  "12345678-1234-1234-1234-123456789002"

BLEServer*         pServer      = nullptr;
BLECharacteristic* pEventsChar  = nullptr;
BLECharacteristic* pControlChar = nullptr;
bool               deviceConnected = false;

// ---------- State ----------
enum State { IDLE, MEASURING };
State currentState = IDLE;

enum MeasurementType { GLUCOSE, KETONE };
MeasurementType currentType = GLUCOSE;

const char* typeName(MeasurementType t) {
  return t == GLUCOSE ? "glucose" : "ketone";
}

// ---------- Measurement ----------
const int            TOTAL_SAMPLES      = 5;
const unsigned long  SAMPLE_INTERVAL_MS = 1000;

int   sampleCount = 0;
float valueSum    = 0;
unsigned long lastSampleTime = 0;

// ---------- Last reading (survives disconnects; app pulls it on connect) ----------
uint32_t        readingId = 0;
MeasurementType lastType  = GLUCOSE;
float           lastValue = 0;

// ---------- Buttons ----------
const unsigned long DEBOUNCE_MS = 300;
unsigned long lastButtonPress = 0;
bool glucoseBtnWasDown = false;
bool ketoneBtnWasDown  = false;

// ---------- Simulated sensor state ----------
float glucoseLevel = 100.0f;
float ketoneLevel  = 0.5f;

// ============================================================
// Simulation (replace with real ADC reads later)
// ============================================================
float simulateSample(MeasurementType t) {
  if (t == GLUCOSE) {
    glucoseLevel += random(-5, 6);
    glucoseLevel = constrain(glucoseLevel, 70.0f, 180.0f);
    return glucoseLevel;
  }
  ketoneLevel += (float)random(-10, 11) / 100.0f;
  ketoneLevel = constrain(ketoneLevel, 0.1f, 3.0f);
  return ketoneLevel;
}

// ============================================================
// Classify
// ============================================================
const char* classify(MeasurementType t, float v) {
  if (t == GLUCOSE) {
    if (v < 70)  return "low";
    if (v > 140) return "high";
    return "normal";
  }
  if (v < 0.5f) return "normal";
  if (v < 1.5f) return "elevated";
  return "high";
}

// ============================================================
// BLE notify
// ============================================================
void notify(const char* payload) {
  if (!deviceConnected) return;
  pEventsChar->setValue(payload);
  pEventsChar->notify();
  Serial.printf("[BLE->] %s\n", payload);
}

// ============================================================
// Message envelope
// { "v":1, "type":"...", "name":"...", "id":"...", "ts":..., "data":{...} }
// ============================================================
void makeMessage(
  char* buf,
  size_t bufSize,
  const char* type,
  const char* name,
  const char* id,        // pass nullptr to omit
  const char* dataJson   // must be valid JSON object e.g. "{}" or "{\"k\":1}"
) {
  if (id != nullptr) {
    snprintf(buf, bufSize,
      "{\"v\":1,\"type\":\"%s\",\"name\":\"%s\",\"id\":\"%s\",\"ts\":%lu,\"data\":%s}",
      type, name, id, millis(), dataJson
    );
  } else {
    snprintf(buf, bufSize,
      "{\"v\":1,\"type\":\"%s\",\"name\":\"%s\",\"ts\":%lu,\"data\":%s}",
      type, name, millis(), dataJson
    );
  }
}

// ============================================================
// Helpers — ack / error / reading payload
// ============================================================
void sendAck(const char* cmdId, const char* name) {
  char buf[256];
  makeMessage(buf, sizeof(buf), "ack", name, cmdId, "{}");
  notify(buf);
}

// cmdId echoes the failing command's id so the app can correlate
void sendError(const char* cmdId, const char* name, const char* reason) {
  char data[64];
  snprintf(data, sizeof(data), "{\"reason\":\"%s\"}", reason);
  char buf[256];
  makeMessage(buf, sizeof(buf), "error", name, cmdId, data);
  notify(buf);
}

// glucose: 1 decimal (mg/dL), ketone: 2 decimals (mmol/L)
void readingJson(char* buf, size_t bufSize) {
  snprintf(buf, bufSize,
    "{\"id\":%lu,\"measurement_type\":\"%s\",\"value\":%.*f,\"status\":\"%s\"}",
    (unsigned long)readingId,
    typeName(lastType),
    lastType == GLUCOSE ? 1 : 2,
    lastValue,
    classify(lastType, lastValue)
  );
}

// ============================================================
// Commands — cmdId is nullptr when triggered by a hardware button
// ============================================================
void startMeasurement(const char* cmdId, MeasurementType type) {
  if (currentState == MEASURING) {
    if (cmdId) sendError(cmdId, "start_measurement", "already_measuring");
    return;
  }

  currentType    = type;
  sampleCount    = 0;
  valueSum       = 0;
  lastSampleTime = millis();
  currentState   = MEASURING;
  digitalWrite(LED_PIN, HIGH);

  if (cmdId) sendAck(cmdId, "start_measurement");

  char data[48];
  snprintf(data, sizeof(data), "{\"measurement_type\":\"%s\"}", typeName(type));
  char buf[256];
  makeMessage(buf, sizeof(buf), "event", "measurement_started", nullptr, data);
  notify(buf);

  Serial.printf("[Measure] started (%s, via %s)\n",
    typeName(type), cmdId ? "BLE" : "button");
}

void cancelMeasurement(const char* cmdId) {
  if (currentState != MEASURING) {
    if (cmdId) sendError(cmdId, "cancel_measurement", "not_measuring");
    return;
  }

  currentState = IDLE;
  digitalWrite(LED_PIN, LOW);

  if (cmdId) sendAck(cmdId, "cancel_measurement");

  char buf[256];
  makeMessage(buf, sizeof(buf), "event", "measurement_cancelled", nullptr, "{}");
  notify(buf);
}

void sendLastReading(const char* cmdId) {
  if (readingId == 0) {
    sendError(cmdId, "get_last_reading", "no_reading_yet");
    return;
  }

  sendAck(cmdId, "get_last_reading");

  char data[192];
  readingJson(data, sizeof(data));
  char buf[320];
  makeMessage(buf, sizeof(buf), "event", "measurement_result", nullptr, data);
  notify(buf);
}

// Lets the app resync after (re)connecting — e.g. mid-measurement
void sendState(const char* cmdId) {
  sendAck(cmdId, "get_state");

  char data[96];
  if (currentState == MEASURING) {
    int progress = (sampleCount * 100) / TOTAL_SAMPLES;
    snprintf(data, sizeof(data),
      "{\"state\":\"measuring\",\"measurement_type\":\"%s\",\"progress\":%d}",
      typeName(currentType), progress);
  } else {
    snprintf(data, sizeof(data), "{\"state\":\"idle\"}");
  }

  char buf[256];
  makeMessage(buf, sizeof(buf), "event", "device_state", nullptr, data);
  notify(buf);
}

// ============================================================
// Command parser
// Expects: {"cmd":"start_measurement","id":"abc123","measurement_type":"ketone"}
// ============================================================
void handleCommand(const std::string& raw) {
  Serial.printf("[CMD<-] %s\n", raw.c_str());

  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, raw);

  if (err) {
    Serial.printf("[CMD] JSON parse error: %s\n", err.c_str());
    sendError("unknown", "unknown_command", "invalid_json");
    return;
  }

  const char* cmd   = doc["cmd"] | "";      // fallback to "" if missing
  const char* cmdId = doc["id"]  | "unknown";

  if (strcmp(cmd, "start_measurement") == 0) {
    const char* typeStr = doc["measurement_type"] | "glucose";
    MeasurementType type = strcmp(typeStr, "ketone") == 0 ? KETONE : GLUCOSE;
    startMeasurement(cmdId, type);
  } else if (strcmp(cmd, "cancel_measurement") == 0) {
    cancelMeasurement(cmdId);
  } else if (strcmp(cmd, "get_last_reading") == 0) {
    sendLastReading(cmdId);
  } else if (strcmp(cmd, "get_state") == 0) {
    sendState(cmdId);
  } else {
    sendError(cmdId, "unknown_command", "not_supported");
  }
}

// ============================================================
// Buttons — fire once per press (falling edge + debounce)
// ============================================================
void handleButtons() {
  bool glucoseDown = digitalRead(BTN_GLUCOSE_PIN) == LOW;
  bool ketoneDown  = digitalRead(BTN_KETONE_PIN)  == LOW;

  unsigned long now = millis();
  bool debounced = (now - lastButtonPress) > DEBOUNCE_MS;

  if (glucoseDown && !glucoseBtnWasDown && debounced) {
    lastButtonPress = now;
    startMeasurement(nullptr, GLUCOSE);
  } else if (ketoneDown && !ketoneBtnWasDown && debounced) {
    lastButtonPress = now;
    startMeasurement(nullptr, KETONE);
  }

  glucoseBtnWasDown = glucoseDown;
  ketoneBtnWasDown  = ketoneDown;
}

// ============================================================
// BLE callbacks
// ============================================================
class ControlCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pChar) override {
    std::string value = pChar->getValue();
    if (value.length() == 0) return;
    handleCommand(value);
  }
};

class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer*) override {
    deviceConnected = true;
    Serial.println("[BLE] connected");
  }

  void onDisconnect(BLEServer* pServer) override {
    deviceConnected = false;
    Serial.println("[BLE] disconnected");
    // NOTE: a running measurement keeps going — button-initiated
    // measurements work offline; app pulls the result on reconnect
    BLEDevice::startAdvertising();
  }
};

// ============================================================
// Setup
// ============================================================
void setup() {
  Serial.begin(115200);
  randomSeed(esp_random());
  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_GLUCOSE_PIN, INPUT_PULLUP);
  pinMode(BTN_KETONE_PIN,  INPUT_PULLUP);

  BLEDevice::init("GlucoKeto-Monitor");
  BLEDevice::setMTU(512);   // must come AFTER init — stack has to be up

  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());

  BLEService* pService = pServer->createService(SERVICE_UUID);

  // Events char — ESP32 notifies app
  pEventsChar = pService->createCharacteristic(
    EVENTS_CHAR,
    BLECharacteristic::PROPERTY_NOTIFY
  );
  pEventsChar->addDescriptor(new BLE2902());

  // Control char — app writes commands to ESP32
  pControlChar = pService->createCharacteristic(
    CONTROL_CHAR,
    BLECharacteristic::PROPERTY_WRITE
  );
  pControlChar->setCallbacks(new ControlCallbacks());

  pService->start();

  BLEDevice::getAdvertising()->addServiceUUID(SERVICE_UUID);
  BLEDevice::getAdvertising()->setScanResponse(true);
  BLEDevice::startAdvertising();

  Serial.println("[BLE] ready — advertising as 'GlucoKeto-Monitor'");
}

// ============================================================
// Loop
// ============================================================
void loop() {
  // Buttons work with or without a phone connected
  handleButtons();

  if (currentState != MEASURING) {
    delay(20);
    return;
  }

  unsigned long now = millis();
  if (now - lastSampleTime < SAMPLE_INTERVAL_MS) return;
  lastSampleTime = now;

  sampleCount++;
  valueSum += simulateSample(currentType);  // swap: real ADC read + calibration

  // Progress event
  int progress = (sampleCount * 100) / TOTAL_SAMPLES;
  char progressData[80];
  snprintf(progressData, sizeof(progressData),
    "{\"progress\":%d,\"measurement_type\":\"%s\"}",
    progress, typeName(currentType));
  char progressBuf[256];
  makeMessage(progressBuf, sizeof(progressBuf), "event", "measurement_progress", nullptr, progressData);
  notify(progressBuf);

  Serial.printf("[Measure] %s sample %d/%d\n", typeName(currentType), sampleCount, TOTAL_SAMPLES);

  if (sampleCount >= TOTAL_SAMPLES) {
    readingId++;
    lastType  = currentType;
    lastValue = valueSum / TOTAL_SAMPLES;

    // Result event
    char resultData[192];
    readingJson(resultData, sizeof(resultData));
    char resultBuf[320];
    makeMessage(resultBuf, sizeof(resultBuf), "event", "measurement_result", nullptr, resultData);
    notify(resultBuf);

    digitalWrite(LED_PIN, LOW);
    currentState = IDLE;
  }
}
