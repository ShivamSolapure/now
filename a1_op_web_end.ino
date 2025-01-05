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

void handleWebPage() {
  String html = R"rawliteral(
    <!DOCTYPE html>
<html>
    <head>
        <title>RMAGEDDON 2025</title>
        <style>
            body {
                margin: 0;
                font-family: 'Orbitron', sans-serif;
                background: linear-gradient(135deg, #1e1e2f, #0f0c29, #302b63, #0f0c29);
                background-size: 300% 300%; 
                animation: gradientShift 10s ease infinite;
                height: 100vh;
                overflow: hidden;
                color: #fff;
            }
            header {
                position: absolute;
                top: 0;
                width: 100%;
                padding: 20px;
                text-align: center;
                background: rgba(0, 0, 0, 0.7);
                box-shadow: 0 4px 10px rgba(0, 0, 0, 0.5);
            }
            h1 {
                font-size: 4.5em;
                margin: 0;
                color: #ffffff;
                text-shadow: 0px 0px 20px rgba(0, 150, 255, 1), 3px 3px 15px rgba(0, 0, 0, 0.8);
                animation: titleGlow 2s infinite alternate;
            }
            h2 {
                font-size: 2.5em;
                margin: 10px 0 20px;
                color: #ffcc00;
                text-shadow: 0px 0px 15px rgba(255, 204, 0, 1), 3px 3px 10px rgba(0, 0, 0, 0.8);
                font-family: 'VT323', monospace;
            }
            main {
                display: flex;
                flex-direction: column;
                justify-content: center;
                align-items: center;
                height: 100%;
                text-align: center;
                animation: fadeIn 3s ease;
            }
            #status {
                font-size: 2.2em;
                margin-top: 20px;
                color: #33ffcc;
                text-shadow: 0px 0px 15px rgba(51, 255, 204, 1), 3px 3px 10px rgba(0, 0, 0, 0.8);
                animation: pulseEffect 3s infinite;
            }
            #timestamp {
                font-size: 1.5em;
                margin-top: 10px;
                color: #ffffff;
                font-family: 'VT323', monospace;
                text-shadow: 0px 0px 10px rgba(255, 255, 255, 0.8);
            }
            footer {
                position: absolute;
                bottom: 10px;
                width: 100%;
                text-align: center;
                font-size: 1.2em;
                color: rgba(255, 255, 255, 0.8);
                font-family: 'VT323', monospace;
            }
            @keyframes gradientShift {
                0% { background-position: 0% 50%; }
                50% { background-position: 100% 50%; }
                100% { background-position: 0% 50%; }
            }
            @keyframes titleGlow {
                0% { text-shadow: 0 0 20px rgba(0, 150, 255, 1), 0 0 30px rgba(0, 150, 255, 0.6); }
                100% { text-shadow: 0 0 30px rgba(0, 150, 255, 1), 0 0 40px rgba(0, 150, 255, 0.8); }
            }
            @keyframes fadeIn {
                from { opacity: 0; }
                to { opacity: 1; }
            }
            @keyframes pulseEffect {
                0%, 100% { transform: scale(1); }
                50% { transform: scale(1.1); }
            }
        </style>
        <script>
            var websocket = new WebSocket('ws://' + window.location.hostname + ':81/');
            var hasShownDetected = false;

            websocket.onmessage = function(event) {
                if (hasShownDetected) return; 

                var data = JSON.parse(event.data);
                if (data.limit_switch_status === "Pressed") {
                    document.getElementById('status').innerHTML = "CHECKPOINT 1: DETECTED";
                    document.getElementById('timestamp').innerHTML = "Timestamp: " + data.timestamp + " ms"; 
                    hasShownDetected = true;
                }
            };
        </script>
    </head>
    <body>
        <header>
            <h1>RMAGEDDON 2025</h1>
        </header>
        <main>
            <h2>Roborace: Team RNXG</h2>
            <p id="status">CHECKPOINT 1:</p>
            <p id="timestamp"></p> <!-- Placeholder for timestamp -->
        </main>
        <footer>
            Innovating the Future | Robotics & Transformation
        </footer>
    </body>
</html>
  )rawliteral";

  server.send(200, "text/html", html);
}


void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Initialization Failed");
    while (true);
  }

  esp_now_register_recv_cb(onDataReceive);
  WiFi.softAP("RMAGEDDON 2025");
  server.on("/", handleWebPage);
  server.begin();

  webSocket.begin();
  Serial.println("Web Server Started");
  Serial.println("Connect to WiFi: RMAGEDDON 2025");
}

void loop() {
  server.handleClient();
  webSocket.loop();
}
