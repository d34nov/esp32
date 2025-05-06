
// The program opens simultaneously WiFi access point
// and BLE device
// Ot also launched WebServer that serves UI html page WebPage.h
// For connecting via WiFi:
// On remote device connect to WiFi access point: "ESP32-C3-AP"
// Type in browser http://192/168.4.1  the UI page will open
//
// For BLE access open in browser page esp32.htm
// NOTE: Must be Chrome on Windows or Android
// Click Connect... button and select BLE device "ESP32-C3-BLE"
// 
// This is an implementation with the Arduino stack

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <NimBLEDevice.h>
#include "NimBleServer.h"
#include "WebPage.h"

#define LED_GPIO 2
#define BTN_GPIO 10

const char *ssid = "ESP32-C3-AP";
const char *password = "";

WebServer server(80);

NimBleServer ble;

// HTTP Handlers
void handleRoot() {
  server.send(200, "text/html", html_page);
}

void handleLEDOn() {
  digitalWrite(LED_GPIO, HIGH);
  Serial.println("LED ON");
  server.send(200, "text/plain", "LED ON");
}

void handleLEDOff() {
  digitalWrite(LED_GPIO, LOW);
  Serial.println("LED OFF");
  server.send(200, "text/plain", "LED OFF");
}

void handleButtonStatus() {
  int btnState = digitalRead(BTN_GPIO);
  String response = (btnState == LOW) ? "Pressed" : "Released";
  Serial.println("HTTP: Button read → " + response);
  server.send(200, "text/plain", response);
}

void initBLE() {
  Serial.println("Initializing NimBLEServer...");
  ble.init((gpio_num_t)LED_GPIO, (gpio_num_t)BTN_GPIO);
  Serial.println("BLE advertising started");
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_GPIO, OUTPUT);
  digitalWrite(LED_GPIO, LOW);

  pinMode(BTN_GPIO, INPUT_PULLUP);

  WiFi.softAP(ssid, password);
  Serial.println("SoftAP started. Connect to SSID: ESP32-C3-AP");

  server.on("/", handleRoot);
  server.on("/on", handleLEDOn);
  server.on("/off", handleLEDOff);
  server.on("/btn", handleButtonStatus);
  server.begin();
  Serial.println("HTTP server started");

  initBLE();
}

void loop() {
  server.handleClient();
  ble.poll();
}
