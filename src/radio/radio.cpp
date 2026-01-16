#include "radio.h"

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

SPIClass customSPI(HSPI);
RF24 radio(CE_PIN, CSN_PIN);
const byte radioAddress[6] = "48FC0";

void initRadio() {
  // Initialize SPI bus FIRST
  customSPI.begin(
    SCK_PIN,
    MISO_PIN,
    MOSI_PIN,
    CSN_PIN
  );

  // Initialize radio with custom SPI bus
  if (!radio.begin(&customSPI, CE_PIN, CSN_PIN)) {
    // optional: handle error
  }

  radio.openWritingPipe(radioAddress);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();
}

void sendTestRadioState(uint8_t a, uint8_t b, bool c, bool d) {
  uint8_t payload[4] = { a, b, c, d };
  radio.write(&payload, sizeof(payload));
  // radio.write(&a, sizeof(a));
}