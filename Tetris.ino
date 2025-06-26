#include "Globals.h"
#include "InputHandler.h"
#include "GameLogic.h"
#include "DisplayManager.h"
#include "StateMachine.h"
#include <EEPROM.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_ADDR 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

TwoWire I2C_MAIN = TwoWire(0);
TwoWire I2C_SIDE = TwoWire(1);
Adafruit_SSD1306 displayMain(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_MAIN, -1);
Adafruit_SSD1306 displaySide(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_SIDE, -1);

// Biến toàn cục (đã được khai báo extern trong Globals.h)
uint8_t board[BOARD_H][BOARD_W] = {0};
uint8_t curPiece, curRot, nextPiece, heldPiece = 255;
int8_t  curX, curY;
bool hasHeldThisTurn = false;
uint32_t score = 0;
uint8_t level = 1;
uint32_t tickMs = 450, lastFallMs = 0;

uint32_t highScores[HS_NUM];

void setup() {
  Serial.begin(115200);

  I2C_MAIN.begin(21, 22);
  I2C_SIDE.begin(19, 18);
  
  if (!displayMain.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR) ||
      !displaySide.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    while (true);
  }

  displayMain.setRotation(1);
  displaySide.setRotation(1);
  displayMain.clearDisplay();
  displaySide.clearDisplay();
  displayMain.display();
  displaySide.display();

  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_ROTATE, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_HOLD, INPUT_PULLUP);

  EEPROM.begin(EEPROM_SIZE);
  loadHighScores();

  randomSeed(esp_random());
  nextPiece = random(0, 7);
  newPiece();
}

void loop() {
  if (btnPressed(btnLeft) && !collide(curX - 1, curY, curRot)) curX--;
  if (btnPressed(btnRight) && !collide(curX + 1, curY, curRot)) curX++;
  if (btnPressed(btnRot)) rotateCW();
  if (btnPressed(btnDown) && !collide(curX, curY + 1, curRot)) curY++;
  if (btnPressed(btnHold)) holdPiece();

  if (millis() - lastFallMs > tickMs) {
    lastFallMs = millis();
    if (!collide(curX, curY + 1, curRot)) {
      curY++;
    } else {
      mergePiece();
      clearLines();
      newPiece();
      if (collide(curX, curY, curRot)) {
        int idx = updateHighScores(score);
        showHighScores(idx);
        while (true) {
          if (!digitalRead(BTN_ROTATE)) {
            delay(200);
            restartGame();
            break;
          }
        }
      }
    }
  }

  drawMain();
  drawSide('N');
}
