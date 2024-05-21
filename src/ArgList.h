#pragma once

#include <Arduino.h>
// #include "CommandHandler.h"

template<typename T>
struct Arg {
    bool error;
    T value;
};

class ArgList {
    private:
        char* args;
        char** savePtr;
        uint8_t numArgs;
        const char delimiter;

        ArgList(char* args, const char delimiter);

        void resetIndex();

    public:
        template<uint8_t, uint8_t, uint8_t> friend class CommandHandler;

        Arg<char*> next();
        Arg<int> nextToInt();
        Arg<float> nextToFloat();
        Arg<bool> nextToBool();
};