// The program opens simultaneously WiFi access point
// and BLE device
// It also launches WebServer that serves UI html page WebPage.h
// For connecting via WiFi:
// On remote device connect to WiFi access point: "ESP32-C3-AP"
// Type in browser http://192.168.4.1  the UI page will open
//
// For BLE access open in browser page esp32.htm
// NOTE: Must be Chrome on Windows or Android
// Click Connect... button and select BLE device "ESP32-C3-BLE"
//
// Both web pages control LED GPIO 2 to turn LED on and off
//and listens to the server events for the Button GPIO 10
// Button is connected between GPIO and  GND (via 5KOM resistor)
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
WiFiClient sseClient;
bool sseConnected = false;
uint8_t lastButtonState = HIGH;

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

// SSE endpoint
void handleEvents() {
  WiFiClient client = server.client();
  if (client && client.connected()) {
    Serial.println("SSE client attempting to connect");

    // Manually write HTTP headers for SSE
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/event-stream");
    client.println("Cache-Control: no-cache");
    client.println("Connection: keep-alive");
    client.println();
    client.flush();  // Ensure headers are sent

    sseClient = client;
    sseConnected = true;
    Serial.println("SSE client connected");
  }
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
  lastButtonState = digitalRead(BTN_GPIO);

  WiFi.softAP(ssid, password);
  Serial.println("SoftAP started. Connect to SSID: ESP32-C3-AP");

  server.on("/", handleRoot);
  server.on("/on", handleLEDOn);
  server.on("/off", handleLEDOff);
  server.on("/events", handleEvents);

  // Catch-all for unknown routes
  server.onNotFound([]() {
    String msg = "404 Not Found: " + server.uri();
    Serial.println("Unknown HTTP request: " + server.uri());
    server.send(404, "text/plain", msg);
  });
  

  server.begin();
  Serial.println("HTTP server started");

  initBLE();
}


void loop() {
  server.handleClient();
  ble.poll();

  // Server-Sent Events button state change
  uint8_t currentState = digitalRead(BTN_GPIO);
  if (sseConnected && currentState != lastButtonState) {
    lastButtonState = currentState;
    
    if (sseClient.connected()) {
      String msg = "data: " + String(currentState == LOW ? "0" : "1") + "\n\n";
      sseClient.print(msg);
     sseClient.flush();
      Serial.println("SSE sent: " + msg);
    } else {
      sseConnected = false;
      Serial.println("SSE client disconnected");
    }
  }
}
