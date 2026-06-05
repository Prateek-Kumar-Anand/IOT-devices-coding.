
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

// ================= TFT =================
#define TFT_CS   15
#define TFT_DC   2
#define TFT_RST  4

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// ================= INPUT =================
#define BTN_LEFT  12
#define BTN_RIGHT 13
#define BTN_ROT   14
#define BTN_DROP  27

// ================= GRID =================
#define GRID_W 10
#define GRID_H 20
#define CELL   6

int grid[GRID_H][GRID_W] = {0};

// ================= GAME STATE =================
int px = 3, py = 0;

int score = 0;
int level = 1;
int linesCleared = 0;

unsigned long lastFall = 0;
int fallSpeed = 500;

// ================= SINGLE PIECE =================
int piece[4][4] = {
  {0,1,0,0},
  {0,1,0,0},
  {0,1,0,0},
  {0,1,0,0}
};

// ================= COLLISION =================
bool collide(int nx, int ny) {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (piece[y][x]) {
        int gx = nx + x;
        int gy = ny + y;

        if (gx < 0 || gx >= GRID_W || gy >= GRID_H) return true;
        if (gy >= 0 && grid[gy][gx]) return true;
      }
    }
  }
  return false;
}

// ================= LOCK PIECE =================
void lockPiece() {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (piece[y][x]) {
        grid[py + y][px + x] = 1;
      }
    }
  }

  px = 3;
  py = 0;
}

// ================= LINE CLEAR =================
void clearLines() {
  for (int y = 0; y < GRID_H; y++) {
    bool full = true;

    for (int x = 0; x < GRID_W; x++) {
      if (!grid[y][x]) {
        full = false;
        break;
      }
    }

    if (full) {
      for (int row = y; row > 0; row--) {
        for (int x = 0; x < GRID_W; x++) {
          grid[row][x] = grid[row - 1][x];
        }
      }

      for (int x = 0; x < GRID_W; x++) {
        grid[0][x] = 0;
      }

      score += 100;
      linesCleared++;
    }
  }

  level = (linesCleared / 5) + 1;
  fallSpeed = max(120, 500 - (level * 40));
}

// ================= MOVE =================
void moveDown() {
  if (!collide(px, py + 1)) {
    py++;
  } else {
    lockPiece();
    clearLines();
  }
}

// ================= ROTATE =================
void rotatePiece() {
  int temp[4][4] = {0};

  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      temp[x][3 - y] = piece[y][x];
    }
  }

  if (!collide(px, py)) {
    memcpy(piece, temp, sizeof(piece));
  }
}

// ================= DRAW =================
void draw() {
  tft.fillScreen(ST77XX_BLACK);

  for (int y = 0; y < GRID_H; y++) {
    for (int x = 0; x < GRID_W; x++) {
      if (grid[y][x]) {
        tft.fillRect(x * CELL, y * CELL, CELL, CELL, ST77XX_BLUE);
      }
    }
  }

  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (piece[y][x]) {
        tft.fillRect((px + x) * CELL, (py + y) * CELL, CELL, CELL, ST77XX_YELLOW);
      }
    }
  }

  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(70, 5);
  tft.print("S:");
  tft.print(score);

  tft.setCursor(70, 20);
  tft.print("L:");
  tft.print(level);
}

// ================= SETUP =================
void setup() {
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_ROT, INPUT_PULLUP);
  pinMode(BTN_DROP, INPUT_PULLUP);

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
}

// ================= LOOP =================
void loop() {
  if (digitalRead(BTN_LEFT) == LOW) {
    if (!collide(px - 1, py)) px--;
    delay(120);
  }

  if (digitalRead(BTN_RIGHT) == LOW) {
    if (!collide(px + 1, py)) px++;
    delay(120);
  }

  if (digitalRead(BTN_ROT) == LOW) {
    rotatePiece();
    delay(150);
  }

  if (digitalRead(BTN_DROP) == LOW) {
    moveDown();
    delay(50);
  }

  if (millis() - lastFall > fallSpeed) {
    moveDown();
    lastFall = millis();
  }

  draw();
}
