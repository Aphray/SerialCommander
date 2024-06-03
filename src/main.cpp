#include <Arduino.h>
#include "CommandHandler.h"

#define EN1 9
#define IN1 8
#define IN2 7

// L298NX2 motors(EN1, IN1, IN2, 6, 5, 4);

MessageHandler<5, 64> msgHandler(&Serial);
CommandHandler<5, 4, 1, 32> cmdHandler(&msgHandler, ',', ':');

int runFor = 0;
// L298N::Direction dir = L298N::FORWARD;

MAKE_CALLBACK(test) {
  Serial.println(name);
  int a1;
  Serial.println(args->getNext(&a1));
  Serial.println(a1);
}

void setup() {
  Serial.begin(9600);
  // cmdHandler.addCommandCallback("test1", &test);
  // cmdHandler.addCommandCallback("test2", &test);
  // cmdHandler.addCommandCallback("test3", &test);

  cmdHandler.addCommandCallback("test", &test);
}

void loop() {
  cmdHandler.process();
  msgHandler.processQueue();
}