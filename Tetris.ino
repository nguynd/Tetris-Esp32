// Tetris_ESP32.ino

#include "Config.h"
#include "Display.h"
#include "Input.h"
#include "GameLogic.h"
#include "HighScores.h"

void waitForRestart() {
  // Vòng lặp chờ người dùng nhấn nút chơi lại
  while (true) {
    if (!digitalRead(BTN_ROTATE)) {
      delay(200); // Chống dội phím đơn giản
      restartGame();
      // Xóa màn hình game over và chuẩn bị cho game mới
      displayMain.clearDisplay();
      displayMain.display();
      break;
    }
  }
}

void setup() {
  Serial.begin(115200);

  // Gọi các hàm setup từ từng module
  setupDisplays();
  setupButtons();
  setupEEPROM();

  loadHighScores();
  
  randomSeed(esp_random());
  newPiece();
}

void loop() {
  // 1. Xử lý input từ người dùng
  handleInput();

  // 2. Logic rơi khối tự động
  if (millis() - lastFallMs > tickMs) {
    lastFallMs = millis();
    
    if (!collide(curX, curY + 1, curRot)) {
      curY++; // Khối rơi xuống
    } else {
      // Khối đã chạm đáy
      mergePiece();
      clearLines();
      newPiece();

      // 3. Kiểm tra Game Over
      if (collide(curX, curY, curRot)) {
        int newRank = updateHighScores(score);
        showHighScores(newRank);
        waitForRestart();
      }
    }
  }

  // 4. Vẽ lại game
  drawMain();
  drawSide();
}