#ifndef NIMBLE_SERVER_H
#define NIMBLE_SERVER_H

#include <NimBLEDevice.h>
#include <driver/gpio.h>

class NimBleServer {
public:
    void init(gpio_num_t ledGpio);
    void poll();  // Call this periodically from loop()

private:
    gpio_num_t _ledGpio;
    NimBLECharacteristic* _ledChar = nullptr;
    std::string _lastValue;
};

#endif // NIMBLE_SERVER_H
