#include <sound.h>
#include <Arduino.h>

void turnOnSound(){
  // C-dur
  tone(PASSIIVE_BUZZER_PIN, 1046.5, 250); // C
  tone(PASSIIVE_BUZZER_PIN, 1318.5, 250); // E
  tone(PASSIIVE_BUZZER_PIN, 1568, 350); // G
}

void beepSound(){
  tone(PASSIIVE_BUZZER_PIN, 1046.5, 100); // C
}