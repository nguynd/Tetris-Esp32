// HighScores.h
#ifndef HIGHSCORES_H
#define HIGHSCORES_H

#include <Arduino.h>

void setupEEPROM();
void loadHighScores();
void saveHighScores();
int updateHighScores(uint32_t s);

#endif // HIGHSCORES_H