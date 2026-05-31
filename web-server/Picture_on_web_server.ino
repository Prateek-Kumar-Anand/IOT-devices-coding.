#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

WebServer server(80);

void handleRoot() {
  String html = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
      <title>ESP32 Image</title>
  </head>
  <body>
      <h2>Image from ESP32</h2>
      <img src="/photo.jpg" width="500">
  </body>
  </html>
  )rawliteral";

  server.send(200, "text/html", html);
}

void handleImage() {
  File file = SPIFFS.open("/photo.jpg", "r");

  if (!file) {
    server.send(404, "text/plain", "Image not found");
    return;
  }

  server.streamFile(file, "image/jpeg");
  file.close();
}

void setup() {
  Serial.begin(115200);

  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/photo.jpg", handleImage);

  server.begin();
}

void loop() {
  server.handleClient();
}