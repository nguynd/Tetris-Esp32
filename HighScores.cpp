// HighScores.cpp
#include <EEPROM.h>
#include "Config.h"
#include "HighScores.h"

uint32_t highScores[HS_NUM];

void setupEEPROM() {
  EEPROM.begin(EEPROM_SIZE);
}

void loadHighScores(){
  for(int i = 0; i < HS_NUM; i++){
    uint32_t v = 0;
    for(int b = 0; b < 4; b++) {
      v |= ((uint32_t)EEPROM.read(i * 4 + b)) << (8 * b);
    }
    highScores[i] = v;
  }
  
  bool invalid = true;
  for (int i = 0; i < HS_NUM; i++) {
    if (highScores[i] != 0xFFFFFFFF && highScores[i] != 0) {
      invalid = false;
      break;
    }
  }
  if (invalid) {
    for (int i = 0; i < HS_NUM; i++) highScores[i] = 0;
    saveHighScores();
  }
}

void saveHighScores(){
  for(int i = 0; i < HS_NUM; i++) {
    for(int b = 0; b < 4; b++) {
      EEPROM.write(i * 4 + b, (highScores[i] >> (8 * b)) & 0xFF);
    }
  }
  EEPROM.commit();
}

int updateHighScores(uint32_t s){
  int idx = -1;
  for(int i = 0; i < HS_NUM; i++) {
    if(s > highScores[i]){
      idx = i;
      break;
    }
  }
  if(idx == -1) return -1;
  
  for(int j = HS_NUM - 1; j > idx; j--) {
    highScores[j] = highScores[j - 1];
  }
  highScores[idx] = s;
  saveHighScores();
  return idx;
}