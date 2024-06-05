#pragma once

#include <Arduino.h>



class CommandData {
    private:
        char* args;
        char** savePtr;
        uint8_t numArgs;
        const char delimiter;

        CommandData(char* args, const char delimiter);

        void resetIndex();

        bool checkNumeric(char* str);

        template <typename T>
        int castNumeric(T (*fn)(const char*), T* ptr) {
            char* buf = getNext();
            if (buf == NULL) 
                return NULL_ARG;
            if (checkNumeric(buf)) 
                *ptr = (*fn)(buf);
            else
                return CONVERSION_ERROR;

            return NO_ERROR;
        };

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
        int getNext(double* ptr);
        int getNext(long* ptr);
        int getNext(bool* ptr);
};