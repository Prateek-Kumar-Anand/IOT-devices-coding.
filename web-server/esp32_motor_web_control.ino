
#include <WiFi.h>
#include <WebServer.h>

// WiFi credentials
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

WebServer server(80);

// Motor pins (L298N)
#define IN1 26
#define IN2 27
#define ENA 14

// PWM setup
const int freq = 1000;
const int pwmChannel = 0;
const int resolution = 8;

int speedValue = 150; // default speed (0-255)

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // PWM setup
  ledcSetup(pwmChannel, freq, resolution);
  ledcAttachPin(ENA, pwmChannel);

  stopMotor();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Routes
  server.on("/", handleRoot);
  server.on("/forward", moveForward);
  server.on("/backward", moveBackward);
  server.on("/stop", stopMotor);
  server.on("/speed", setSpeed);

  server.begin();
}

void loop() {
  server.handleClient();
}

// ---------- Motor Functions ----------
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  ledcWrite(pwmChannel, speedValue);
  server.send(200, "text/plain", "Motor Forward");
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  ledcWrite(pwmChannel, speedValue);
  server.send(200, "text/plain", "Motor Backward");
}

void stopMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  ledcWrite(pwmChannel, 0);
  server.send(200, "text/plain", "Motor Stopped");
}

// Speed control: /speed?value=0-255
void setSpeed() {
  if (server.hasArg("value")) {
    speedValue = server.arg("value").toInt();
    speedValue = constrain(speedValue, 0, 255);
  }
  server.send(200, "text/plain", "Speed set to " + String(speedValue));
}

// ---------- Web Page ----------
void handleRoot() {
  String html = R"rawliteral(
  <html>
  <head>
    <title>ESP32 Motor Control</title>
    <style>
      body { text-align:center; font-family:Arial; margin-top:50px; }
      button { padding:15px 25px; margin:10px; font-size:18px; }
    </style>
  </head>
  <body>
    <h2>ESP32 Motor Control</h2>

    <button onclick="fetch('/forward')">Forward</button>
    <button onclick="fetch('/backward')">Backward</button>
    <button onclick="fetch('/stop')">Stop</button>

    <br><br>

    <input type="range" min="0" max="255" onchange="setSpeed(this.value)">
    <p>Speed Control</p>

    <script>
      function setSpeed(val){
        fetch('/speed?value=' + val);
      }
    </script>
  </body>
  </html>
  )rawliteral";

  server.send(200, "text/html", html);
}
