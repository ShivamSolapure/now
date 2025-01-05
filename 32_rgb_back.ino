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
                justify-content: flex-start;
                align-items: center;
                height: 100vh;
                margin: 0;
                text-align: center;
                font-weight: bold;
                background: linear-gradient(120deg, #ff5733, #ffcc33, #33ccff, #9933ff); /* Gradient background */
                background-size: 400% 400%; /* Animate gradient movement */
                animation: gradientAnimation 10s ease infinite; /* Smooth animation */
                color: white;
                text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.8);
            }
            h1 {
                font-size: 4em;
                margin: 20px 0;
            }
            h2 {
                font-size: 2.5em;
                margin: 10px 0;
            }
            #status {
                font-size: 1.5em;
                margin-top: 15px;
            }
            #timestamp {
                font-size: 1.2em;
                margin-top: 10px;
                font-weight: normal;
            }
            @keyframes gradientAnimation {
                0% { background-position: 0% 50%; }
                50% { background-position: 100% 50%; }
                100% { background-position: 0% 50%; }
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
