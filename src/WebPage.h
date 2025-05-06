// WebPage.h
#pragma once
#include <Arduino.h> 

const char html_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32-C3 LED</title>
</head>
<body>
  <h1>ESP32-C3 LED Control</h1>
  <button onclick="fetch('/on')">LED ON</button>
  <button onclick="fetch('/off')">LED OFF</button>
</body>
</html>
)rawliteral";
