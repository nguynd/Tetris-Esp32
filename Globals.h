#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

#define BTN_LEFT   32
#define BTN_RIGHT  33
#define BTN_ROTATE 25
#define BTN_DOWN   26
#define BTN_HOLD   27

extern Adafruit_SSD1306 displayMain;
extern Adafruit_SSD1306 displaySide;

extern uint8_t board[20][10];
extern uint8_t curPiece, curRot, nextPiece, heldPiece;
extern int8_t curX, curY;
extern bool hasHeldThisTurn;
extern uint32_t score;
extern uint8_t level;
extern uint32_t tickMs, lastFallMs;

#endif
