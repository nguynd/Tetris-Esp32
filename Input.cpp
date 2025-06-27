// Input.cpp
#include "Config.h"
#include "Input.h"
#include "GameLogic.h"

// Định nghĩa các biến nút bấm
Btn btnLeft{BTN_LEFT}, btnRight{BTN_RIGHT}, btnRot{BTN_ROTATE}, btnDown{BTN_DOWN}, btnHold{BTN_HOLD};

void setupButtons() {
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_ROTATE, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_HOLD, INPUT_PULLUP);
}

// Hàm này được handleInput sử dụng nội bộ
bool btnPressed(Btn &b){
  bool now = !digitalRead(b.pin);
  if(now && !b.prev && (millis() - b.last > DEBOUNCE_MS)){
    b.last = millis();
    b.prev = now;
    return true;
  }
  b.prev = now;
  return false;
}

void handleInput() {
  if(btnPressed(btnLeft)  && !collide(curX - 1, curY, curRot)) curX--;
  if(btnPressed(btnRight) && !collide(curX + 1, curY, curRot)) curX++;
  if(btnPressed(btnRot))   rotateCW();
  if(btnPressed(btnDown)  && !collide(curX, curY + 1, curRot)) curY++;
  if(btnPressed(btnHold))  holdPiece();
}