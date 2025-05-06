The project requires PlatformIO with Espressif 32 platform installed.

The board used is ESP32-C3 ( generic There is no such board in the PIO library).

All other settings are in platformio.ini file.

The firmware opens simultaneously WiFi access point and BLE device.

It also launches WebServer that serves UI html page WebPage.h
 
For connecting via WiFi:
On remote device connect to WiFi access point: "ESP32-C3-AP"
Type in browser http://192.168.4.1  the UI page will open

 For BLE access open in browser page esp32.html
NOTE: Must be Chrome on Windows or Android
Click Connect... button and select BLE device "ESP32-C3-BLE"
(soemtimes it can show up as Unrecognised. In this case use MAC address to identify the device)

Both web pages control LED GPIO 2 to turn LED on and off
and listens to the server events for the Button GPIO 10
The Button is connected between GPIO and  GND (via 5KOM resistor)
