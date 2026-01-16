#include <Arduino.h>
#include <input.h>
#include <GEM_u8g2.h>

InputState inputState = {};
bool wasButtonPressed[BUTTONS_COUNT];
unsigned long lastButtonsUpdateTime = 0;

uint8_t range255(int value){
  if(value > 255) return 255;
  else if(value < 0) return 0;
  return value;
}

void registerButtonPress(GEM_u8g2* menu, uint8_t button){
  if(button == static_cast<uint8_t>(Button::up)) menu->registerKeyPress(GEM_KEY_UP);
  else if(button == static_cast<uint8_t>(Button::down)) menu->registerKeyPress(GEM_KEY_DOWN);
  else if(button == static_cast<uint8_t>(Button::left)) menu->registerKeyPress(GEM_KEY_LEFT);
  else if(button == static_cast<uint8_t>(Button::right)) menu->registerKeyPress(GEM_KEY_RIGHT);
  else if(button == static_cast<uint8_t>(Button::ok)) menu->registerKeyPress(GEM_KEY_OK);
  else if(button == static_cast<uint8_t>(Button::cancel)) menu->registerKeyPress(GEM_KEY_CANCEL);
}

// TODO also check button presses for UART
char lineBuf[128];
uint8_t lineLen = 0;
void readSwitchesInput(){
  while (Serial2.available()) {
    uint8_t c = Serial2.read();

    if (c == 0x80) {
      inputState.switch2State[0] = lineBuf[0] == '1';
      inputState.switch2State[1] = lineBuf[1] == '1';
      lineLen = 0;
    }
    else if (lineLen < LINE_MAX - 7) {
      for(uint8_t i = 0; i < 7; i++){
        // checing each bit
        lineBuf[lineLen + i] = (c & static_cast<uint8_t>(1u << i)) ? '1' : '0';
      }
      lineLen += 7;
    }
    else {
      // Overflow protection
      lineLen = 0;
    }
  }
}

void readInput(GEM_u8g2* menu){
  unsigned long currentMillis = millis();
  readSwitchesInput();

  // register button presses, implementation of a 10ms delay to avoid fake button presses (noise)
  if(currentMillis - lastButtonsUpdateTime >= 10){
    lastButtonsUpdateTime = currentMillis;
    for(uint8_t i = 0; i < BUTTONS_COUNT; i++){
      const bool pressed = digitalRead(buttonsPins[i]) == LOW ? true : false;
      const bool wasPressed = wasButtonPressed[i];
      
      // if the button was just released
      if(!pressed && wasPressed){
        // If menu is ready to accept button press, register it
        if(i < 6 && menu->readyForKey()){
          registerButtonPress(menu, i);
          Serial.println("button press registered");
        }
        inputState.buttonPressed[i] = true;
        wasButtonPressed[i] = false;
      }

      // if the button was just pressed
      else{
        inputState.buttonPressed[i] = false;
        if(pressed && !wasPressed) wasButtonPressed[i] = true;
      }
    }
  }

  // read potentiometers values
  // Serial.println(range255(map(analogRead(36), 3210, 7, 0, 255)));
  // Serial.println(range255(map(analogRead(39), 3650, 375, 0, 255)));
  // Serial.println();
  inputState.potentiometerState[static_cast<int>(Potentiometer::secondVertical)] = range255(map(analogRead(34), 4095, 770, 0, 255));
  inputState.potentiometerState[static_cast<int>(Potentiometer::secondHorizontal)] = range255(map(analogRead(35), 3430, 300, 0, 255));
  inputState.potentiometerState[static_cast<int>(Potentiometer::firstVertical)] = range255(map(analogRead(36), 3200, 7, 0, 255));
  inputState.potentiometerState[static_cast<int>(Potentiometer::firstHorizontal)] = range255(map(analogRead(39), 3650, 375, 0, 255));
}

