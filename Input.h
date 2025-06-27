// Input.h
#ifndef INPUT_H
#define INPUT_H

#include <Arduino.h>

struct Btn {
  const uint8_t pin;
  bool prev = false;
  uint32_t last = 0;
};

void setupButtons();
void handleInput();

#endif // INPUT_H