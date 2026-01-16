#include <Arduino.h>
#include <GEM_u8g2.h>
#include <LittleFS.h>
#include <input.h>
#include <sound.h>
#include <filesystem.h>
#include "navigation/navigation.h"
#include "radio/radio.h"

void setup() {
  Serial.begin(115200);

  // activate pull-up resistors for all buttons; TODO: remove when UART
  for(uint8_t i = 0; i < BUTTONS_COUNT; i++){
    pinMode(buttonsPins[i], INPUT_PULLUP);
  }

  // initialize LittleFS and check if the models directory exists
  if(!initFilesystem()){
    Serial.println("Filesystem initialization failed");
    return;
  };

  // set passive buzzer pin mode & play the turn-on sound
  pinMode(PASSIIVE_BUZZER_PIN, OUTPUT);
  if(systemSettings.soundEffects) turnOnSound();

  initMenu();

  Serial2.setPins(19, NOT_A_PIN);
  initRadio();
}

void loop() {
  readInput(&menu);

  const uint8_t a = inputState.potentiometerState[static_cast<int>(Potentiometer::secondVertical)];
  const uint8_t b = inputState.potentiometerState[static_cast<int>(Potentiometer::secondHorizontal)];
  const bool c = inputState.switch2State[static_cast<int>(Switch2::A)];
  const bool d = inputState.switch2State[static_cast<int>(Switch2::B)];
  sendTestRadioState(a, b, c, d);
  Serial.println(a);

  delay(15);
  // Serial.println(inputState.potentiometerState[static_cast<int>(Potentiometer::secondVertical)]);
  // Serial.println(inputState.potentiometerState[static_cast<int>(Potentiometer::secondHorizontal)]);
  // Serial.println();
  // delay(200);
}

/*
TODO
#include <> -> ""
*/