# Setup Arduino IDE cho board ESP32 Cheap Yellow Display (CYD)

Board dùng trong repo này: **ESP32-2432S028R** (còn gọi là "Cheap Yellow Display" / CYD).
Repo tham khảo: https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display

## 1. Cài driver USB

Board dùng chip CH340 để giao tiếp USB-UART. Nếu Windows không tự nhận cổng COM khi
cắm board, cài driver CH340 theo hướng dẫn:
https://learn.sparkfun.com/tutorials/how-to-install-ch340-drivers/all

## 2. Cài ESP32 board core cho Arduino IDE

1. Mở **File > Preferences**, thêm URL sau vào ô "Additional Boards Manager URLs":
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
2. Mở **Tools > Board > Boards Manager**, tìm "esp32" (Espressif Systems) và cài.
3. Chọn board: **Tools > Board > ESP32 Arduino > ESP32-2432S028R** (core bản mới có sẵn
   entry này; nếu core cũ hơn không có, chọn "ESP32 Dev Module" thay thế).
4. Chọn đúng **Port** (COM tương ứng khi cắm board).
5. Nếu upload báo lỗi kết nối, thử hạ **Upload Speed** xuống `115200`.

## 3. Cài thư viện TFT_eSPI và cấu hình pin cho CYD

Đây là bước quan trọng nhất — thiếu bước này sẽ gặp lỗi `TFT_eSPI.h: No such file or
directory` khi biên dịch, hoặc màn hình lên nguồn nhưng **tối om / không sáng
backlight** dù code chạy đúng.

1. **Sketch > Include Library > Manage Libraries...**, tìm và cài **TFT_eSPI** (tác giả
   Bodmer).
2. Tìm thư mục thư viện vừa cài, thường ở:
   ```
   Documents\Arduino\libraries\TFT_eSPI\
   ```
3. Tải file cấu hình pin riêng cho CYD tại:
   https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display/blob/main/DisplayConfig/User_Setup.h
4. **Ghi đè** file `User_Setup.h` mặc định trong thư mục `TFT_eSPI` ở bước 2 bằng file
   vừa tải (mặc định thư viện cấu hình sẵn cho ESP8266 NodeMCU, sai hoàn toàn pin và
   không bật backlight cho board CYD).
5. Đóng và mở lại Arduino IDE để nạp lại thư viện.

File `User_Setup.h` đúng cho CYD có các thông số chính:
- Driver: `ILI9341_2_DRIVER`
- Pin SPI: `MISO=12, MOSI=13, SCLK=14, CS=15, DC=2, RST=-1`
- Backlight: `TFT_BL=21`, `TFT_BACKLIGHT_ON HIGH`

## 4. Upload và kiểm tra

1. Mở sketch (ví dụ `lcd-read-data/lcd-read-data.ino`).
2. Chọn đúng Board + Port như bước 2.
3. Bấm Upload.
4. Sau khi upload xong, màn hình sẽ tự sáng và hiển thị nội dung — không cần code gì
   thêm để bật backlight vì `TFT_BL`/`TFT_BACKLIGHT_ON` đã cấu hình sẵn trong
   `User_Setup.h`.

## Troubleshooting

- **Lỗi biên dịch `TFT_eSPI.h: No such file or directory`**: chưa cài thư viện
  TFT_eSPI (bước 3.1).
- **Upload được nhưng màn hình tối om**: `User_Setup.h` trong thư viện TFT_eSPI chưa
  được thay bằng bản của CYD (bước 3.3–3.4), hoặc chưa restart Arduino IDE sau khi
  thay file.
- **Không thấy cổng COM khi cắm board**: kiểm tra lại driver CH340 (bước 1), thử cáp
  USB khác (một số cáp chỉ sạc, không truyền data), hoặc thử cổng USB khác trên máy
  tính.
