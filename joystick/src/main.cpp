#include <Arduino.h>
#include <WiFiUdp.h>
#include <WiFi.h>
#include "arduino_secrets.h"
#include "JoystickPacket.h"

#define DELAY 50

#define UP 0
#define DWN 1
#define LET 2
#define RHT 3
#define MID 4
#define SET 5

WiFiUDP udp;
IPAddress broadcastIP(255, 255, 255, 255);

struct JoystickStatus {
  int x, y, mid, set;

  bool operator==(const JoystickStatus& other) const {
    return x == other.x && y == other.y && mid == other.mid && set == other.set;
  }

  bool operator!=(const JoystickStatus& other) const {
    return x != other.x || y != other.y || mid != other.mid || set != other.set;
  }
};

JoystickStatus cur_st;
JoystickStatus prv_st;
JoystickPacket outgoing;
unsigned long lastSend = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(UP, INPUT_PULLUP);
  pinMode(DWN, INPUT_PULLUP);
  pinMode(LET, INPUT_PULLUP);
  pinMode(RHT, INPUT_PULLUP);
  pinMode(MID, INPUT_PULLUP);
  pinMode(SET, INPUT_PULLUP);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.printf("\nWiFi OK, IP: %s\n", WiFi.localIP().toString().c_str());
}

void updateJoystickStatus() {
  cur_st.y = !digitalRead(UP) ? 1 : (!digitalRead(DWN) ? -1 : 0);
  cur_st.x = !digitalRead(RHT) ? 1 : (!digitalRead(LET) ? -1 : 0);
  cur_st.mid = !digitalRead(MID) ? 1 : 0;
  cur_st.set = !digitalRead(SET) ? 1 : 0;
}

void loop() {
  if (millis() - lastSend >= DELAY) {
    updateJoystickStatus();
    prv_st = cur_st;
    outgoing = JoystickPacket{ (int16_t)cur_st.x, (int16_t)cur_st.y, (uint8_t)cur_st.mid, (uint8_t)cur_st.set };

    lastSend = millis();
    udp.beginPacket(broadcastIP, UDP_PORT);
    udp.write((uint8_t*)&outgoing, sizeof(outgoing));
    udp.endPacket();
  }
}
