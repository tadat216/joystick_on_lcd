/*******************************************************************
    Hello World for the ESP32 Cheap Yellow Display.

    https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display

    If you find what I do useful and would like to support me,
    please consider becoming a sponsor on Github
    https://github.com/sponsors/witnessmenow/

    Written by Brian Lough
    YouTube: https://www.youtube.com/brianlough
    Twitter: https://twitter.com/witnessmenow
 *******************************************************************/

// Cau hinh pin/driver cho man hinh duoc truyen qua build_flags trong
// platformio.ini (khong can ghi de User_Setup.h trong thu vien nua).

// Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
// Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
// Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
// Note the following larger fonts are primarily numeric only!
// Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
// Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:-.
// Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.

#include <TFT_eSPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <esp_wifi.h>
#include "arduino_secrets.h"
#include "JoystickPacket.h"

TFT_eSPI tft = TFT_eSPI();
WiFiUDP udp;
JoystickPacket incoming, prev_incoming;

void readMacAddress() {
  uint8_t baseMac[6];
  esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
  if (ret == ESP_OK) {
    Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
                  baseMac[0], baseMac[1], baseMac[2],
                  baseMac[3], baseMac[4], baseMac[5]);
  } else {
    Serial.println("Failed to read MAC address");
  }
}

void displayHelloWorld() {
  // Clear the screen before writing to it
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  int x = 5;
  int y = 10;
  int fontNum = 2;
  tft.drawString("Hello", x, y, fontNum);  // Left Aligned
  x = 320 / 2;
  y += 16;
  tft.setTextColor(TFT_BLUE, TFT_BLACK);
  tft.drawCentreString("World", x, y, fontNum);
}

void setup() {
  Serial.begin(115200);

  // Start the tft display and set it to black
  tft.init();
  tft.setRotation(1);  //This is the display in landscape

  displayHelloWorld();

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.printf("\nWiFi OK, IP: %s\n", WiFi.localIP().toString().c_str());

  Serial.print("[DEFAULT] ESP32 Board MAC Address: ");
  readMacAddress();

  udp.begin(UDP_PORT);
  Serial.printf("Dang lang nghe UDP tren port %d\n", UDP_PORT);
}

void loop() {
  int packetSize = udp.parsePacket();
  if (packetSize == sizeof(incoming)) {
    udp.read((uint8_t*)&incoming, sizeof(incoming));
    Serial.printf("[NHAN] tu %s: x=%d y=%d mid=%d set=%d\n",
                  udp.remoteIP().toString().c_str(),
                  incoming.x, incoming.y, incoming.midPressed, incoming.setPressed);
  } else if (packetSize > 0) {
    Serial.printf("Nhan goi tin la, size=%d (mong doi %d)\n", packetSize, (int)sizeof(incoming));
  }
}
