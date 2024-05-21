#include <Arduino.h>
#include "CommandHandler.h"
#include "MessageHandler.h"

MessageHandler<5, 64> mh;
CommandHandler<5, 5, 64> ch(',', ':', '<', '>', &mh);


void setup() {
}

void loop() {

}
