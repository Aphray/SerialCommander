#pragma once

#include <Arduino.h>


// template<typename T>
// struct ArgBase {
//     bool error;
//     T value;
// };

// template<typename T>
// struct Arg: ArgBase<T> {};

// template<> 
// struct Arg<char*>: ArgBase<char*> {
//     Arg<int> toInt() {
        
//     }

//     Arg<float> toFloat() {

//     }

//     Arg<bool> toBool() {

//     }

//     private:

// };


class CommandData {
    private:
        char* args;
        char** savePtr;
        uint8_t numArgs;
        const char delimiter;

        CommandData(char* args, const char delimiter);

        void resetIndex();

        bool checkNumeric(char* str);

        char* getNext();

    public:
        enum ErrorCode {
            NO_ERROR,
            NULL_ARG,
            CONVERSION_ERROR
        };

        template<uint8_t, uint8_t, uint8_t, uint8_t> friend class CommandHandler;

        char* getName();
        uint8_t getNumArgs();

        int getNext(char* ptr);
        int getNext(int* ptr);
        int getNext(float* ptr);
        int getNext(long* ptr);
        int getNext(bool* ptr);
};