void handleWebPage() {
  String html = R"rawliteral(
    <!DOCTYPE html>
<html>
    <head>
        <title>RMAGEDDON 2025</title>
        <style>
            body {
                display: flex;
                flex-direction: column;
                justify-content: center;
                align-items: center;
                height: 100vh;
                margin: 0;
                text-align: center;
                font-family: 'Arial', sans-serif;
                background: linear-gradient(135deg, #0f0c29, #302b63, #24243e); /* Dark tech gradient */
                background-size: 400% 400%; /* Smooth animated gradient */
                animation: gradientAnimation 15s ease infinite;
                color: #ffffff;
                text-shadow: 3px 3px 10px rgba(0, 0, 0, 0.8); /* Subtle shadow for text */
            }

            h1 {
                font-size: 4.5em;
                margin: 20px 0;
                color: #ffffff;
                text-shadow: 0px 0px 20px rgba(0, 150, 255, 1), 3px 3px 10px rgba(0, 0, 0, 0.8); /* Neon glow effect */
            }

            h2 {
                font-size: 2.8em;
                margin: 15px 0;
                color: #ffcc00;
                text-shadow: 0px 0px 15px rgba(255, 204, 0, 1), 3px 3px 10px rgba(0, 0, 0, 0.8); /* Yellow glow */
            }

            #status {
                font-size: 2em;
                margin-top: 20px;
                color: #33ffcc;
                text-shadow: 0px 0px 15px rgba(51, 255, 204, 1), 3px 3px 10px rgba(0, 0, 0, 0.8); /* Aqua glow */
            }

            #timestamp {
                font-size: 1.5em;
                margin-top: 10px;
                color: #ffffff;
                font-weight: normal;
                text-shadow: 0px 0px 10px rgba(255, 255, 255, 0.8);
            }

            @keyframes gradientAnimation {
                0% { background-position: 0% 50%; }
                50% { background-position: 100% 50%; }
                100% { background-position: 0% 50%; }
            }

            button {
                font-size: 1.5em;
                margin-top: 20px;
                padding: 10px 20px;
                border: none;
                border-radius: 5px;
                color: #ffffff;
                background: linear-gradient(90deg, #ff5733, #ffcc33, #33ccff);
                cursor: pointer;
                text-shadow: 0px 0px 10px rgba(0, 0, 0, 0.5);
                box-shadow: 0px 5px 15px rgba(0, 150, 255, 0.5);
                transition: transform 0.2s, box-shadow 0.2s;
            }

            button:hover {
                transform: scale(1.1);
                box-shadow: 0px 10px 20px rgba(255, 204, 0, 0.8);
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
        <button onclick="alert('Good luck, Team RNXG!')">Cheer for Us!</button>
    </body>
</html>
  )rawliteral";

  server.send(200, "text/html", html);
}