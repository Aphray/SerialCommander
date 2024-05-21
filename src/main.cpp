#include <Arduino.h>
#include "CommandHandler.h"
#include "MessageHandler.h"

MessageHandler<5, 64> mh;
CommandHandler<5, 5, 64> ch(',', ':', '<', '>', &mh);

void test(char* name, ArgList* args) {

}

void test2(char* name, ArgList* args) {

}

void setup() {

    ch.addCommandCallback(F("TEST"), &test);
    ch.addCommandCallback(F("TEST2"), &test);
    ch.addCommandCallback(F("TEST"), &test2);
    ch.addCommandCallback(F("TEST2"), &test2);
}

void loop() {

}
