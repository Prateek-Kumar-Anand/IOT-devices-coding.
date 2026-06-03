#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);

String getHTML() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    return "<h1>DHT11 Read Failed!</h1>";
  }

  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta http-equiv="refresh" content="2">
<title>ESP32 Weather Station</title>
<style>
body{
  font-family:Arial;
  text-align:center;
  background:#f0f0f0;
  margin-top:50px;
}
.card{
  width:320px;
  margin:auto;
  background:white;
  padding:20px;
  border-radius:15px;
  box-shadow:0px 0px 15px rgba(0,0,0,0.2);
}
.value{
  font-size:40px;
  font-weight:bold;
}
</style>
</head>
<body>
<div class="card">
<h1>ESP32 DHT11 Monitor</h1>
<p>Temperature</p>
<div class="value">)rawliteral";

  html += String(t);
  html += R"rawliteral( °C</div>

<p>Humidity</p>
<div class="value">)rawliteral";

  html += String(h);
  html += R"rawliteral( %</div>

</div>
</body>
</html>
)rawliteral";

  return html;
}

void handleRoot() {
  server.send(200, "text/html", getHTML());
}

void setup() {
  Serial.begin(115200);

  dht.begin();

  WiFi.begin(ssid, password);

  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();

  Serial.println("Web Server Started");
}

void loop() {
  server.handleClient();
}