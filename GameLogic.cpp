// GameLogic.cpp
#include "GameLogic.h"

// Định nghĩa các biến trạng thái game  
uint8_t board[BOARD_H][BOARD_W] = {0};
uint8_t curPiece, curRot, nextPiece, heldPiece = 255;
int8_t  curX, curY;
bool hasHeldThisTurn = false;
uint32_t score = 0;
uint8_t level = 1;
uint32_t tickMs = 450, lastFallMs = 0;

// Định nghĩa mảng tetromino
const uint16_t tetromino[7][4] = {
  {0x0F00,0x2222,0x00F0,0x4444}, {0x0660,0x0660,0x0660,0x0660},
  {0x0E40,0x4C40,0x4E00,0x4640}, {0x06C0,0x8C40,0x06C0,0x8C40},
  {0x0C60,0x4C80,0x0C60,0x4C80}, {0x8E00,0x6440,0x0E20,0x44C0},
  {0x2E00,0x4460,0x0E80,0xC440}
};

void newPiece(){
  curPiece = nextPiece;
  nextPiece = random(0, 7);
  curRot = 0;
  curX = 3;
  curY = 0;
  hasHeldThisTurn = false;
}

bool collide(int8_t nx, int8_t ny, uint8_t nRot){
  uint16_t s = tetromino[curPiece][nRot];
  for(int8_t y = 0; y < 4; y++) {
    for(int8_t x = 0; x < 4; x++) {
      if(s & (0x8000 >> (y * 4 + x))){
        int8_t bx = nx + x, by = ny + y;
        if(bx < 0 || bx >= BOARD_W || by >= BOARD_H) return true;
        if(by >= 0 && board[by][bx]) return true;
      }
    }
  }
  return false;
}

void mergePiece(){
  uint16_t s = tetromino[curPiece][curRot];
  for(int8_t y = 0; y < 4; y++) {
    for(int8_t x = 0; x < 4; x++) {
      if(s & (0x8000 >> (y * 4 + x))){
        int8_t bx = curX + x, by = curY + y;
        if(by >= 0 && by < BOARD_H && bx >= 0 && bx < BOARD_W) board[by][bx] = curPiece + 1;
      }
    }
  }
}

void clearLines(){
  uint8_t lines = 0;
  for(int8_t y = BOARD_H - 1; y >= 0; y--){
    bool full = true;
    for(int8_t x = 0; x < BOARD_W; x++) {
      if(!board[y][x]){
        full = false;
        break;
      }
    }
    if(full){
      for(int8_t yy = y; yy > 0; yy--) {
        memcpy(board[yy], board[yy - 1], BOARD_W);
      }
      memset(board[0], 0, BOARD_W);
      y++;
      lines++;
    }
  }
  switch(lines){
    case 1: score += 100; break;
    case 2: score += 250; break;
    case 3: score += 500; break;
    case 4: score += 1000; break;
  }
  level = score / 2000 + 1;
  tickMs = 450 - (level - 1) * 30;
  if(tickMs < 100) tickMs = 100;
}

void rotateCW(){
  uint8_t n = (curRot + 1) & 3;
  if(!collide(curX, curY, n)) curRot = n;
}

void holdPiece(){
  if(hasHeldThisTurn) return;
  if(heldPiece == 255){
    heldPiece = curPiece;
    newPiece();
  } else {
    uint8_t t = curPiece;
    curPiece = heldPiece;
    heldPiece = t;
    curRot = 0;
    curX = 3;
    curY = 0;
  }
  hasHeldThisTurn = true;
}

void restartGame(){
  memset(board, 0, sizeof(board));
  score = 0;
  level = 1;
  tickMs = 450;
  heldPiece = 255;
  hasHeldThisTurn = false;
  nextPiece = random(0, 7);
  newPiece();
}