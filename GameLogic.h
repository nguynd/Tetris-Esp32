// GameLogic.h
#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <Arduino.h>
#include "Config.h"

// Khai báo các biến trạng thái game
extern uint8_t board[BOARD_H][BOARD_W];
extern uint8_t curPiece, curRot, nextPiece, heldPiece;
extern int8_t  curX, curY;
extern bool hasHeldThisTurn;
extern uint32_t score;
extern uint8_t level;
extern uint32_t tickMs, lastFallMs;
extern const uint16_t tetromino[7][4];

// Khai báo các hàm logic
void newPiece();
bool collide(int8_t nx, int8_t ny, uint8_t nRot);
void mergePiece();
void clearLines();
void rotateCW();
void holdPiece();
void restartGame();

#endif // GAMELOGIC_H