#include "CommandData.h"
#include <stdlib.h>



CommandData::CommandData(char* args, const char delimiter): delimiter(delimiter) {

    uint8_t n = 0;
    while (args[n] != 0) {
        if (args[n] == delimiter) numArgs++;
        n++;
    }
    numArgs++;
    this->args = args;
    resetIndex();
}

void CommandData::resetIndex() {
    savePtr = &args;
}

bool CommandData::checkNumeric(char* str) {
    uint8_t decimals = 0;

    if (str == NULL) return false;

    for (uint8_t n = 0; n < strlen(str); n++) {
        if (!isdigit(str[n])) return false;

        switch(str[n]) {
            case '-':
            case '+':
                if (n != 0) return false;
                break;
            case '.':
                if (++decimals > 1) return false;
                break;
        }
    }

    return true;
}


char* CommandData::getNext() {
    return strtok_r(args, &delimiter, savePtr);
}

int CommandData::getNext(char* ptr) {
    ptr = getNext();
    if (ptr == NULL) return NULL_ARG;
    return NO_ERROR;
}

int CommandData::getNext(int* ptr) {
    // char* buf = getNext();
    // if (buf == NULL) return NULL_ARG;
    // if (_checkNumeric(buf)) {
    //     *ptr = atoi(buf);
    // } else {
    //     return CONVERSION_ERROR;
    // }

    // return NO_ERROR;
    return castNumeric(&atoi, ptr);
}

int CommandData::getNext(double* ptr) {
    // char* buf = getNext();
    // if (buf == NULL) return NULL_ARG;
    // if (_checkNumeric(buf)) {
    //     *ptr = atof(buf);
    // } else {
    //     return CONVERSION_ERROR;
    // }
    
    // return NO_ERROR;
    return castNumeric(&atof, ptr);
}

int CommandData::getNext(long* ptr) {
    // char* buf = getNext();
    // if (buf == NULL) return NULL_ARG;
    // if (_checkNumeric(buf)) {
    //     *ptr = atol(buf);
    // } else {
    //     return CONVERSION_ERROR;
    // }
    
    // return NO_ERROR;
    return castNumeric(&atol, ptr);
}

int CommandData::getNext(bool* ptr) {
    int val;
    int error = getNext(&val);

    if (error != NO_ERROR) return error;

    switch(val) {
        case 1:
            *ptr = true;
            break;
        case 0:
            *ptr = false;
            break;
        default:
            return CONVERSION_ERROR;
    }

    return NO_ERROR;
}