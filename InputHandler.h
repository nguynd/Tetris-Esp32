#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <Arduino.h>

struct Btn {
  const uint8_t pin;
  bool prev = false;
  uint32_t last = 0;
};

extern Btn btnLeft, btnRight, btnRot, btnDown, btnHold;
bool btnPressed(Btn &b);

#endif
