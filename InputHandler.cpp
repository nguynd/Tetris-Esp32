#include "InputHandler.h"
#include "Globals.h"

const uint16_t DEBOUNCE_MS = 120;
Btn btnLeft{BTN_LEFT}, btnRight{BTN_RIGHT}, btnRot{BTN_ROTATE}, btnDown{BTN_DOWN}, btnHold{BTN_HOLD};

bool btnPressed(Btn &b) {
  bool now = !digitalRead(b.pin);
  if (now && !b.prev && millis() - b.last > DEBOUNCE_MS) {
    b.last = millis();
    b.prev = now;
    return true;
  }
  b.prev = now;
  return false;
}
