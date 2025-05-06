#include "NimBleServer.h"
#include <Arduino.h>

static const char *SERVICE_UUID = "0000180f-0000-1000-8000-00805f9b34fb";
static const char *CHAR_UUID =    "00002a19-0000-1000-8000-00805f9b34fb";

void NimBleServer::init(gpio_num_t ledGpio) {
    _ledGpio = ledGpio;

    pinMode(_ledGpio, OUTPUT);
    digitalWrite(_ledGpio, LOW);

    NimBLEDevice::init("ESP32-C3-BLE");
    Serial.print("BLE MAC: ");
    Serial.println(NimBLEDevice::getAddress().toString().c_str());

    NimBLEServer *pServer = NimBLEDevice::createServer();
    NimBLEService *pService = pServer->createService(SERVICE_UUID);

    _ledChar = pService->createCharacteristic(
        CHAR_UUID,
        NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::WRITE_NR
    );

    _ledChar->setValue("OFF");
    _lastValue = "OFF";

    pService->start();

    NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->start();

    Serial.println("BLE advertising started");
}

void NimBleServer::poll() {
    if (!_ledChar) return;

    std::string value = _ledChar->getValue();
    if (value != _lastValue) {
        Serial.print("BLE command received: ");
        Serial.println(value.c_str());
        _lastValue = value;

        if (value == "ON") {
            digitalWrite(_ledGpio, HIGH);
            Serial.println("LED turned ON");
        } else if (value == "OFF") {
            digitalWrite(_ledGpio, LOW);
            Serial.println("LED turned OFF");
        } else {
            Serial.println("Unknown BLE command");
        }
    }
}
