#include <WiFi.h>
#include <esp_now.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

WebServer server(80);
WebSocketsServer webSocket(81); // WebSocket server on port 81

volatile bool limitSwitchStatus = false;

// Callback function to handle received data
void onDataReceive(const esp_now_recv_info *info, const uint8_t *data, int len) {
  if (len > 0) {
    limitSwitchStatus = data[0];
    Serial.print("Limit Switch Status Received: ");
    Serial.println(limitSwitchStatus ? "Pressed" : "Not Pressed");

    // Only send "Pressed" status to WebSocket clients
    if (limitSwitchStatus) {
      StaticJsonDocument<200> jsonDoc;

      // Add limit switch status and timestamp
      jsonDoc["limit_switch_status"] = "Pressed";
      jsonDoc["timestamp"] = millis(); // Add the timestamp in milliseconds

      String jsonString;
      serializeJson(jsonDoc, jsonString);
      webSocket.broadcastTXT(jsonString); // Send JSON to all clients
    }
  }
}

// Serve the HTML page to the client
void handleWebPage() {
  String html = R"rawliteral(
    <!DOCTYPE html>
<html>
    <head>
        <title>RMAGEDDON 2025</title>
        <title>Roborace: Team RnXG</title>
        <style>
            body {
                display: flex;
                flex-direction: column;
                justify-content: flex-start; /* Start from the top */
                align-items: center;
                height: 100vh;
                margin: 0;
                text-align: center;
                font-weight: bold; /* Make all text bold */
                background-image: url('https://via.placeholder.com/1920x1080'); /* Replace with your image URL */
                background-size: cover; /* Ensure the image covers the full page */
                background-position: center; /* Center the image */
                color: gray; /* Ensure text is readable on the image */
                text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.8); /* Add text shadow for better readability */
            }
            h1 {
                font-size: 4em; /* Slightly increased size */
                margin: 20px 0; /* Decreased top and bottom margins */
            }
            h2 {
                font-size: 2.5em; /* Slightly increased size */
                margin: 10px 0; /* Reduced margin for closer spacing to h1 */
            }
            #status {
                font-size: 1.5em; /* Keep bold status text */
                margin-top: 15px; /* Slightly reduced top margin */
            }
            #timestamp {
                font-size: 1.2em; /* Not bold for the timestamp */
                margin-top: 10px;
                font-weight: normal; /* Ensure timestamp is not bold */
            }
        </style>
        <script>
            var websocket = new WebSocket('ws://' + window.location.hostname + ':81/');
            var hasShownDetected = false;

            websocket.onmessage = function(event) {
                if (hasShownDetected) return; // Ignore further updates if "DETECTED" has been shown
                
                var data = JSON.parse(event.data);
                if (data.limit_switch_status === "Pressed") {
                    document.getElementById('status').innerHTML = "CHECKPOINT 1: DETECTED";
                    document.getElementById('timestamp').innerHTML = "Timestamp: " + data.timestamp + " ms"; // Display the timestamp
                    hasShownDetected = true; // Lock the state to "DETECTED"
                }
            };
        </script>
    </head>
    <body>
        <h1>RMAGEDDON 2025</h1>
        <h2>Roborace: Team RNXG</h2>
        <p id="status">CHECKPOINT 1:</p>
        <p id="timestamp"></p> <!-- Placeholder for timestamp -->
    </body>
</html>


)rawliteral";

  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);

  // Set WiFi to Station mode
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Initialization Failed");
    while (true);
  }

  // Register callback for receiving data
  esp_now_register_recv_cb(onDataReceive);

  // Start a WiFi Access Point for the web server
  WiFi.softAP("RMAGEDDON 2025");

  // Start the web server
  server.on("/", handleWebPage);
  server.begin();

  // Start the WebSocket server
  webSocket.begin();

  Serial.println("Web Server Started");
  Serial.println("Connect to WiFi: RMAGEDDON 2025");
}

void loop() {
  // Handle incoming HTTP requests
  server.handleClient();

  // Handle WebSocket communication
  webSocket.loop();
}

