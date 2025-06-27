// Config.h
#ifndef CONFIG_H
#define CONFIG_H
#include <Arduino.h>

// --- Cấu hình màn hình ---
#define OLED_ADDR       0x3C
#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64

// --- Cấu hình chân I2C ---
#define I2C_MAIN_SDA    21
#define I2C_MAIN_SCL    22
#define I2C_SIDE_SDA    19
#define I2C_SIDE_SCL    18

// --- Cấu hình chân nút bấm ---
#define BTN_LEFT        32
#define BTN_RIGHT       33
#define BTN_ROTATE      25
#define BTN_DOWN        26
#define BTN_HOLD        27

// --- Cấu hình game ---
const uint8_t BOARD_W = 10;
const uint8_t BOARD_H = 20;
const uint8_t PIX = 5;
const uint16_t DEBOUNCE_MS = 120;

// --- Cấu hình High Score ---
#define HS_NUM          5
#define EEPROM_SIZE     64

#endif // CONFIG_H