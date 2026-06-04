#include <Adafruit_GFX.h> #include <Adafruit_ST7735.h> #include <SPI.h>
#include “DHT.h”

#define DHTPIN 4 #define DHTTYPE DHT11 DHT dht(DHTPIN, DHTTYPE)

#define TFT_CS 5 #define TFT_DC 16 #define TFT_RST 17

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

#define GRAPH_WIDTH 128 #define GRAPH_HEIGHT 80 #define GRAPH_X 0
#define GRAPH_Y 40

float tempData[GRAPH_WIDTH]; float humData[GRAPH_WIDTH];

void drawAxes(); void drawGraph();

void setup() { Serial.begin(115200); dht.begin();

tft.initR(INITR_BLACKTAB); tft.setRotation(1);
tft.fillScreen(ST77XX_BLACK);

tft.setTextColor(ST77XX_WHITE); tft.setTextSize(1);

tft.setCursor(10, 5); tft.print(“ESP32 DHT11 Graph”);

drawAxes(); }

void loop() { float temp = dht.readTemperature(); float hum =
dht.readHumidity();

if (isnan(temp) || isnan(hum)) { delay(1000); return; }

for (int i = 0; i < GRAPH_WIDTH - 1; i++) { tempData[i] = tempData[i +
1]; humData[i] = humData[i + 1]; }

tempData[GRAPH_WIDTH - 1] = temp; humData[GRAPH_WIDTH - 1] = hum;

drawGraph(); delay(1000); }

void drawAxes() { tft.drawRect(GRAPH_X, GRAPH_Y, GRAPH_WIDTH,
GRAPH_HEIGHT, ST77XX_WHITE); }

void drawGraph() { tft.fillRect(GRAPH_X + 1, GRAPH_Y + 1, GRAPH_WIDTH -
2, GRAPH_HEIGHT - 2, ST77XX_BLACK);

for (int i = 1; i < GRAPH_WIDTH; i++) { int x1 = i - 1; int x2 = i;

    int y1 = GRAPH_Y + GRAPH_HEIGHT - map(tempData[i - 1], 0, 50, 0, GRAPH_HEIGHT);
    int y2 = GRAPH_Y + GRAPH_HEIGHT - map(tempData[i], 0, 50, 0, GRAPH_HEIGHT);
    tft.drawLine(x1, y1, x2, y2, ST77XX_RED);

    int y3 = GRAPH_Y + GRAPH_HEIGHT - map(humData[i - 1], 0, 100, 0, GRAPH_HEIGHT);
    int y4 = GRAPH_Y + GRAPH_HEIGHT - map(humData[i], 0, 100, 0, GRAPH_HEIGHT);
    tft.drawLine(x1, y3, x2, y4, ST77XX_CYAN);

} }
