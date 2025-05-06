// WebPage.h
#pragma once
#include <Arduino.h>

const char html_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32-C3 LED and Button</title>
  <style>
    body { font-family: sans-serif; padding: 20px; }
    button { padding: 10px; margin: 5px; font-size: 16px; }
    #status { margin-top: 15px; font-weight: bold; }
  </style>
</head>
<body>
  <h1>ESP32-C3 LED and Button Control</h1>

  <h3>LED Control</h3>
  <button onclick="fetch('/on')"> LED ON</button>
  <button onclick="fetch('/off')"> LED OFF</button>

  <h3>Button State</h3>
  <p>Button is: <span id="btnState">Unknown</span></p>

  <script>
    function updateButtonState() {
      fetch('/btn')
        .then(response => response.text())
        .then(text => {
          document.getElementById("btnState").textContent = text;
        })
        .catch(err => {
          document.getElementById("btnState").textContent = "Error";
        });
    }

    setInterval(updateButtonState, 1000); // poll every second
  </script>
</body>
</html>
)rawliteral";
