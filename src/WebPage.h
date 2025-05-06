// WebPage.h
#pragma once
#include <Arduino.h>

const char html_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32-C3 LED + Button</title>
  <meta charset="utf-8">
  <style>
    body { font-family: sans-serif; padding: 20px; }
    button { margin: 5px; padding: 10px; font-size: 16px; }
    #status { font-weight: bold; margin-top: 15px; }
    #raw { color: gray; margin-top: 5px; font-size: 14px; }
  </style>
</head>
<body>
  <h1>ESP32-C3 LED + Button Control</h1>
  <button onclick="fetch('/on')">💡 LED ON</button>
  <button onclick="fetch('/off')">❌ LED OFF</button>

  <p id="status">Button: unknown</p>
  <div id="raw">[raw event: none]</div>

  <script>
    const statusEl = document.getElementById("status");
    const rawEl = document.getElementById("raw");

    const evtSource = new EventSource("/events");

    evtSource.onmessage = function(event) {
      console.log("SSE event received:", event.data);
      rawEl.textContent = "[raw event: " + event.data + "]";
      if (event.data === "0") {
        statusEl.textContent = "Button: Pressed";
      } else if (event.data === "1") {
        statusEl.textContent = "Button: Released";
      } else {
        statusEl.textContent = "Button: Unknown data";
      }
    };

    evtSource.onerror = function(err) {
      console.error("SSE connection error:", err);
      rawEl.textContent = "[SSE error occurred]";
    };
  </script>
</body>
</html>
)rawliteral";
