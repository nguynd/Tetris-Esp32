// Display.cpp
#include <Wire.h>
#include "Config.h"
#include "Display.h"
#include "GameLogic.h"
#include "HighScores.h" // Cần để truy cập mảng highScores

// Khởi tạo các đối tượng I2C và màn hình
TwoWire I2C_MAIN = TwoWire(0);
TwoWire I2C_SIDE = TwoWire(1);
Adafruit_SSD1306 displayMain(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_MAIN, -1);
Adafruit_SSD1306 displaySide(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_SIDE, -1);

// Cần truy cập mảng highScores từ module HighScores
extern uint32_t highScores[HS_NUM];

void setupDisplays() {
  I2C_MAIN.begin(I2C_MAIN_SDA, I2C_MAIN_SCL);
  I2C_SIDE.begin(I2C_SIDE_SDA, I2C_SIDE_SCL);

  if (!displayMain.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR) || !displaySide.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }
  displayMain.setRotation(1);
  displaySide.setRotation(1);
  displayMain.clearDisplay();
  displaySide.clearDisplay();
  displayMain.display();
  displaySide.display();
}

void drawMain(){
  const uint8_t X0 = (SCREEN_HEIGHT - BOARD_W * PIX) / 2;
  const uint8_t Y0 = (SCREEN_WIDTH - BOARD_H * PIX);

  displayMain.clearDisplay();
  for(int8_t y = 0; y < BOARD_H; y++) {
    for(int8_t x = 0; x < BOARD_W; x++) {
      if(board[y][x]) displayMain.fillRect(X0 + x * PIX, Y0 + y * PIX, PIX, PIX, SSD1306_WHITE);
    }
  }
  
  uint16_t s = tetromino[curPiece][curRot];
  for(int8_t y = 0; y < 4; y++) {
    for(int8_t x = 0; x < 4; x++) {
      if(s & (0x8000 >> (y * 4 + x))){
        int bx = curX + x, by = curY + y;
        if(by >= 0) displayMain.fillRect(X0 + bx * PIX, Y0 + by * PIX, PIX, PIX, SSD1306_WHITE);
      }
    }
  }
  
  displayMain.drawRect(X0 - 1, Y0 - 1, BOARD_W * PIX + 2, BOARD_H * PIX + 2, SSD1306_WHITE);
  displayMain.display();
}

void drawSide(){
  displaySide.clearDisplay();
  displaySide.setRotation(1);
  displaySide.setTextColor(SSD1306_WHITE);

  displaySide.setTextSize(1);
  displaySide.setCursor(0, 0);
  displaySide.print("SCORE");
  displaySide.setTextSize(2);
  displaySide.setCursor(0, 8);
  displaySide.println(score);

  displaySide.setTextSize(1);
  displaySide.setCursor(0, 26);
  displaySide.print("LEVEL");
  displaySide.setTextSize(2);
  displaySide.setCursor(0, 34);
  displaySide.println(level);

  displaySide.setTextSize(1);
  displaySide.setCursor(0, 55);
  displaySide.print("NEXT");
  uint16_t ns = tetromino[nextPiece][0];
  for(int8_t y = 0; y < 4; y++) {
    for(int8_t x = 0; x < 4; x++) {
      if(ns & (0x8000 >> (y * 4 + x))) displaySide.fillRect(40 + x * 5, 55 + y * 5, 5, 5, SSD1306_WHITE);
    }
  }

  displaySide.setTextSize(1);
  displaySide.setCursor(0, 85);
  displaySide.print("HOLD");
  if(heldPiece != 255){
    uint16_t hs = tetromino[heldPiece][0];
    for(int8_t y = 0; y < 4; y++) {
      for(int8_t x = 0; x < 4; x++) {
        if(hs & (0x8000 >> (y * 4 + x))) displaySide.fillRect(40 + x * 5, 85 + y * 5, 5, 5, SSD1306_WHITE);
      }
    }
  }
  displaySide.display();
}

void showHighScores(int highlight){
  displayMain.clearDisplay();
  displayMain.setTextSize(1);
  displayMain.setTextColor(SSD1306_WHITE);
  displayMain.setCursor(12, 0); displayMain.print("GAME OVER!");
  displayMain.setCursor(18, 12); displayMain.print("TOP 5");
  for(int i = 0; i < HS_NUM; i++){
    int y = 24 + i * 8;
    if(i == highlight){
      displayMain.setCursor(0, y);
      displayMain.print((char)0x10); // Ký tự mũi tên ►
    }
    displayMain.setCursor(6, y);
    displayMain.print(String(i + 1));
    displayMain.print(". ");
    displayMain.print(highScores[i]);
  }
  displayMain.display();
}