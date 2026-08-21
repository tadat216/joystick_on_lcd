#pragma once

#include <cstdint>

// UDP port dung chung giua joystick sender va LCD receiver.
constexpr uint16_t UDP_PORT = 4210;

typedef struct __attribute__((packed)) {
  int16_t x;
  int16_t y;
  uint8_t midPressed;
  uint8_t setPressed;
} JoystickPacket;
