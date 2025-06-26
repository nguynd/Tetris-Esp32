# 🎮 ESP32 Dual OLED Tetris Game

## 📘 Mô tả

Đây là một phiên bản Tetris được triển khai trên **ESP32**, sử dụng **hai màn hình OLED (I2C)** để hiển thị đồng thời:
- Bảng trò chơi chính
- Thông tin phụ: điểm số, level, khối tiếp theo (Next), khối đang giữ (Hold)

Chức năng nổi bật:
- Điều khiển bằng 5 nút bấm: Trái, Phải, Xoay, Xuống, và Giữ
- Lưu **top 5 điểm cao nhất** vào **EEPROM**
- Hiển thị bảng xếp hạng khi kết thúc trò chơi
- Bắt đầu lại trò chơi bằng cách nhấn nút “Xoay”

---

## 🧰 Phần cứng yêu cầu

| Thiết bị             | Ghi chú                                |
|---------------------|----------------------------------------|
| ESP32 DevKit v1     | (hoặc board ESP32 tương thích)         |
| 2x Màn hình OLED I2C| Độ phân giải 128x64                    |
| Nút nhấn (5 cái)     | Kết nối vào các chân GPIO              |
| Dây nối & Breadboard| Hoặc PCB tự làm tùy vào thiết kế       |

---

## 🧱 Sơ đồ chân (GPIO)

| Nút bấm  | GPIO |
|----------|------|
| Trái     | 32   |
| Phải     | 33   |
| Xoay     | 25   |
| Xuống    | 26   |
| Giữ      | 27   |

OLED:
- Màn hình chính: SDA = GPIO 21, SCL = GPIO 22
- Màn hình phụ: SDA = GPIO 19, SCL = GPIO 18

---

## 🧰 Thư viện cần thiết

Cài đặt các thư viện sau trong Arduino IDE:

- `Adafruit SSD1306`
- `Adafruit GFX`
- `EEPROM`
- `Wire`

---

## ⚙️ Cài đặt & nạp chương trình

### 1. Cài đặt môi trường ESP32
1. Mở Arduino IDE
2. Thêm link vào `Preferences > Additional Board URLs`:
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
3. Cài đặt board `ESP32` từ `Tools > Board > Board Manager`

### 2. Cấu hình Arduino IDE

- **Board**: `ESP32 Dev Module`
- **Port**: chọn cổng tương ứng
- **Baudrate**: `115200`

### 3. Mở và nạp code

1. Mở file `Tetris.ino` trong Arduino IDE
2. Cắm ESP32 và chọn đúng port
3. Nhấn `Upload` để nạp chương trình

> Nếu gặp lỗi nạp, hãy nhấn giữ nút `BOOT` trên ESP32 khi bắt đầu nạp.

---

## 🕹️ Hướng dẫn chơi

- **Trái (GPIO 32):** Di chuyển khối sang trái
- **Phải (GPIO 33):** Di chuyển sang phải
- **Xoay (GPIO 25):** Xoay khối
- **Xuống (GPIO 26):** Tăng tốc rơi
- **Giữ (GPIO 27):** Giữ khối hiện tại

Khi game over:
- Bảng điểm hiện ra
- Nhấn nút **Xoay (GPIO 25)** để bắt đầu lại

---

## 🧪 Kiểm tra và debug

- Mở Serial Monitor (115200 baud) để xem log
- Dùng `EEPROM.begin(EEPROM_SIZE)` để lưu dữ liệu điểm

---

## 📷 Giao diện mẫu

> (Bạn có thể thêm hình ảnh minh họa nếu muốn)

---

## 📌 Ghi chú

- Game sử dụng bộ font và tọa độ thủ công để hiển thị
- Chơi tốt nhất với 2 màn hình OLED 128x64
- EEPROM lưu điểm cao vẫn được giữ sau khi tắt nguồn

---

## 🛠️ Tác giả

Nguyễn Trần Thái Dương - duong.ntt225822@sis.hust.edu.vn
Lê Đình Quốc Huy - huy.ldq225857@sis.hust.edu.vn