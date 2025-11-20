#include <Arduino.h>
#include <input.h>
#include <GEM_u8g2.h>

InputState inputState = {};
bool wasButtonPressed[BUTTONS_COUNT];
unsigned long lastButtonsUpdateTime = 0;

void registerButtonPress(GEM_u8g2* menu, uint8_t button){
  if(button == static_cast<uint8_t>(Button::up)) menu->registerKeyPress(GEM_KEY_UP);
  else if(button == static_cast<uint8_t>(Button::down)) menu->registerKeyPress(GEM_KEY_DOWN);
  else if(button == static_cast<uint8_t>(Button::left)) menu->registerKeyPress(GEM_KEY_LEFT);
  else if(button == static_cast<uint8_t>(Button::right)) menu->registerKeyPress(GEM_KEY_RIGHT);
  else if(button == static_cast<uint8_t>(Button::ok)) menu->registerKeyPress(GEM_KEY_OK);
  else if(button == static_cast<uint8_t>(Button::cancel)) menu->registerKeyPress(GEM_KEY_CANCEL);
}

void readInput(GEM_u8g2* menu){
  unsigned long currentMillis = millis();
  
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
}
