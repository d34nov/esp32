#ifndef NIMBLE_SERVER_H
#define NIMBLE_SERVER_H

#include <NimBLEDevice.h>
#include <driver/gpio.h>

class NimBleServer {
public:
    void init(gpio_num_t ledGpio, gpio_num_t buttonGpio);
    void poll();  // Called from loop()

private:
    gpio_num_t _ledGpio;
    gpio_num_t _buttonGpio;

    NimBLECharacteristic* _ledChar = nullptr;
    NimBLECharacteristic* _buttonChar = nullptr;
    std::string _lastValue;
    uint8_t _lastButtonState = 1; // HIGH = not pressed (due to pull-up)

    void updateButtonStatus();
};

#endif // NIMBLE_SERVER_H