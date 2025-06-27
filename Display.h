// Display.h
#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Khai báo các đối tượng hiển thị
extern Adafruit_SSD1306 displayMain;
extern Adafruit_SSD1306 displaySide;

// Khai báo các hàm
void setupDisplays();
void drawMain();
void drawSide();
void showHighScores(int highlight);

#endif // DISPLAY_H