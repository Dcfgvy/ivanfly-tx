#pragma once

#include <SPI.h>
#include <RF24.h>

#define CE_PIN 23
#define CSN_PIN 22
#define SCK_PIN 21
#define MISO_PIN 16
#define MOSI_PIN 17

extern SPIClass customSPI;
extern RF24 radio;

void initRadio();

void sendTestRadioState(uint8_t a, uint8_t b, bool c, bool d);
