#pragma once
#include <cstdint>
#include <GEM_u8g2.h>

// TODO change to 14 when reading from UART
#define BUTTONS_COUNT 6
// TODO: remove when reading from UART
static constexpr uint8_t buttonsPins[BUTTONS_COUNT] = {25, 14, 27, 33, 32, 26};

enum class Button : uint8_t {
  up = 0,
  down = 1,
  left = 2,
  right = 3,
  ok = 4,
  cancel = 5,
  trim1Up = 6,
  trim1Down = 7,
  trim1Left = 8,
  trim1Right = 9,
  trim2Up = 10,
  trim2Down = 11,
  trim2Left = 12,
  trim2Right = 13,
};

enum class Switch2 : uint8_t {
  A = 0,
  B = 1,
  C = 2,
  D = 3,
};

enum class Switch3 : uint8_t {
  E = 0,
  F = 1,
  G = 2,
  H = 3,
};

enum class Potentiometer : uint8_t {
  firstVertical = 0,
  firstHorizontal = 1,
  secondVertical = 2,
  secondHorizontal = 3,
  variable1 = 4,
  variable2 = 5,
};

// Example: static_cast<uint8_t>(Button::up)

struct InputState {
  bool buttonPressed[14];
  // bool buttonHeld[14];
  bool switch2State[4]; /* 0: down, 1: up */
  uint8_t switch3State[4]; /* 0: down, 1: middle, 2: up */
  uint8_t potentiometerState[6]; /* range 0-255 */
};

extern InputState inputState;

void readInput(GEM_u8g2* menu);