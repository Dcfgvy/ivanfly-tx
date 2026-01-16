// This is the code of an additional microcontroller (ATTiny1614 in this case) for reading button/switch inputs in order to save pins on the main MCU.
// This code should be uploaded directly to the ATTiny (e.g. in Arduino IDE with megaTinyCore)

#include <Arduino.h>

#define BAUD_RATE 115200
#define ROWS_COUNT 4
#define COLUMNS_COUNT 6
#define UART_DATA_BYTES 3
#define UART_FRAME_DATA_BITS 7 // we need MSB to always be unset in order to identify the end of a message (0x80 byte)
#define MESSAGE_END_BYTE 0x80

const static uint8_t ROWS[ROWS_COUNT] = {PIN_PB0, PIN_PB1, PIN_PA1, PIN_PA2};
const static uint8_t COLUMNS[COLUMNS_COUNT] = {PIN_PB3, PIN_PA3, PIN_PA7, PIN_PA6, PIN_PA5, PIN_PA4};

uint8_t state[UART_DATA_BYTES]; // state of all switches, 1 if closed, 0 otherwise

void setup() {
  for(uint8_t i = 0; i < COLUMNS_COUNT; i++) pinMode(COLUMNS[i], INPUT);
  for(uint8_t i = 0; i < ROWS_COUNT; i++){
    pinMode(ROWS[i], OUTPUT);
    digitalWrite(ROWS[i], LOW);
  }

  Serial.pins(PIN_PB2 /* TX: default TX pin */, NOT_A_PIN /* RX: disable RX as we need the pin for other purposes */);
  Serial.begin(BAUD_RATE, SERIAL_8N1);
}

void loop() {
  // Clearing the state
  memset(state, 0, UART_DATA_BYTES);

  for(uint8_t i = 0; i < ROWS_COUNT; i++){
    digitalWrite(ROWS[i], HIGH);
    delayMicroseconds(100);
    for(uint8_t j = 0; j < COLUMNS_COUNT; j++){
      if(i == 3 && j > 2) break; // we don't use the last 3 switches
      if(digitalRead(COLUMNS[j])){
        // bit-packing
        uint8_t n = COLUMNS_COUNT * i + j;
        state[n / UART_FRAME_DATA_BITS] |= static_cast<uint8_t>(1u << (n % UART_FRAME_DATA_BITS));
      }
    }
    digitalWrite(ROWS[i], LOW);
  }

  // 0.1ms * 4 in the loop + 0.1ms = 0.5ms pause between updates;
  // (11 * 4) bits * (1000 / 0.5) updates = 11kB/s throughput
  // delayMicroseconds(100);
  delay(5);

  // Sending updates
  for(uint8_t i = 0; i < UART_DATA_BYTES; i++) Serial.write(state[i]);
  Serial.write(MESSAGE_END_BYTE);
}
