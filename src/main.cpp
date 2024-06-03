#include <Arduino.h>

#define RX_BUFFER_SIZE 128
#define TX_BUFFER_SIZE 256

#include "CommandHandler.h"


auto a = TX_BUFFER_SIZE;

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println(RX_BUFFER_SIZE);
  Serial.println(TX_BUFFER_SIZE);
}