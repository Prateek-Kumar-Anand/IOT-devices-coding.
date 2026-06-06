#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

// ====== WiFi Credentials ======
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// ====== ST7735 Pins ======
#define TFT_CS   5
#define TFT_DC   2
#define TFT_RST  4

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
WebServer server(80);

// ====== HTML Chat Page ======
const char MAIN_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 Chat</title>
  <style>
    body { font-family: Arial; text-align:center; background:#111; color:white; }
    input { padding:10px; width:70%; }
    button { padding:10px; }
  </style>
</head>
<body>
  <h2>ESP32 Chat to TFT</h2>
  <input id="msg" placeholder="Type message">
  <button onclick="sendMsg()">Send</button>

  <script>
    function sendMsg(){
      var xhttp = new XMLHttpRequest();
      var msg = document.getElementById("msg").value;
      xhttp.open("GET", "/send?text=" + encodeURIComponent(msg), true);
      xhttp.send();
      document.getElementById("msg").value = "";
    }
  </script>
</body>
</html>
)rawliteral";

void showOnTFT(String text) {
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 20);
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(1);
  tft.println("Message:");
  tft.println(text);
}

void handleRoot() {
  server.send(200, "text/html", MAIN_page);
}

void handleSend() {
  if (server.hasArg("text")) {
    String msg = server.arg("text");
    Serial.println(msg);
    showOnTFT(msg);
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "No message");
  }
}

void setup() {
  Serial.begin(115200);

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_GREEN);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0,0);
  tft.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/send", handleSend);
  server.begin();
}

void loop() {
  server.handleClient();
}
