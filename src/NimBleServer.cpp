#include "NimBleServer.h"
#include <Arduino.h>

static const char *SERVICE_UUID = "0000180f-0000-1000-8000-00805f9b34fb";
static const char *CHAR_LED_UUID = "00002a19-0000-1000-8000-00805f9b34fb";
static const char *CHAR_BTN_UUID = "00002a1a-0000-1000-8000-00805f9b34fb"; // custom UUID

void NimBleServer::init(gpio_num_t ledGpio, gpio_num_t buttonGpio) {
    _ledGpio = ledGpio;
    _buttonGpio = buttonGpio;

    pinMode(_ledGpio, OUTPUT);
    digitalWrite(_ledGpio, LOW);

    pinMode(_buttonGpio, INPUT_PULLUP);

    NimBLEDevice::init("ESP32-C3-BLE");
    Serial.print("BLE MAC: ");
    Serial.println(NimBLEDevice::getAddress().toString().c_str());

    NimBLEServer *pServer = NimBLEDevice::createServer();
    NimBLEService *pService = pServer->createService(SERVICE_UUID);

    _ledChar = pService->createCharacteristic(
        CHAR_LED_UUID,
        NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::WRITE_NR
    );
    _ledChar->setValue("OFF");

    _buttonChar = pService->createCharacteristic(
        CHAR_BTN_UUID,
        NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY
    );
    _buttonChar->setValue("1");  // Default HIGH (not pressed)

    pService->start();

    NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->start();

    Serial.println("BLE advertising started");
}

void NimBleServer::poll() {
    if (_ledChar) {
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

    updateButtonStatus();
}

void NimBleServer::updateButtonStatus() {
    if (!_buttonChar) return;
    uint8_t current = digitalRead(_buttonGpio);
    if (current != _lastButtonState) {
        _lastButtonState = current;
        _buttonChar->setValue(&current, 1);
        _buttonChar->notify();
        Serial.print("NBLES: Button state changed: ");
        Serial.println(current == 0 ? "Pressed" : "Released");
    }
}
