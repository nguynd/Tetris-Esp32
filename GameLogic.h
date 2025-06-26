#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

void newPiece();
void rotateCW();
void clearLines();
void mergePiece();
bool collide(int8_t x, int8_t y, uint8_t r);
void holdPiece();

#endif
