#include "CommandData.h"
#include <stdlib.h>



bool _checkNumeric(char* str) {
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


CommandData::CommandData(char* args, const char delimiter): delimiter(delimiter) {

    ErrorCode error = NO_ERROR;

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

// bool CommandData::checkNumeric(char* str) {
//     if (error == NO_ERROR) {
//         if (_checkNumeric(str)) {
//             return true;
//         } else {
//             error = CONVERSION_ERROR;
//         }
//     }

//     return false;
// }

// char* CommandData::getNext() {
//     char* arg = strtok_r(args, &delimiter, savePtr);
//     error = (arg == NULL) ? NULL_ARG : NO_ERROR;
//     return arg;
// }

// int CommandData::getNextAsInt() {
//     char* arg = getNext();
//     if (error == NULL_ARG) return 0;
//     return checkNumeric(arg) ? atoi(arg): 0;
// }

// float CommandData::getNextAsFloat() {
//     char* arg = getNext();
//     if (error == NULL_ARG) return 0.0;
//     return checkNumeric(arg) ? atof(arg) : 0.0;
// }

// long CommandData::getNextAsLong() {
//     char* arg = getNext();
//     if (error == NULL_ARG) return 0L;
//     return checkNumeric(arg) ? atol(arg) : 0L;
// }

// bool CommandData::getNextAsBool() {
//     int val = getNextAsInt();
//     if (error == CONVERSION_ERROR) return false;

//     switch(val) {
//         case 1:
//             return true;
//         case 0:
//             return false;
//         default:
//             error = CONVERSION_ERROR;
//             break;
//     }

//     return false;
// }

// bool CommandData::noError() {
//     return error == NO_ERROR;
// }

char* CommandData::getNext() {
    return strtok_r(args, &delimiter, savePtr);
}

int CommandData::getNext(char* ptr) {
    ptr = getNext();
    if (ptr == NULL) return NULL_ARG;
    return NO_ERROR;
}

int CommandData::getNext(int* ptr) {
    char* buf = getNext();
    if (buf == NULL) return NULL_ARG;
    if (_checkNumeric(buf)) {
        *ptr = atoi(buf);
    } else {
        return CONVERSION_ERROR;
    }

    return NO_ERROR;
}

int CommandData::getNext(float* ptr) {
    char* buf = getNext();
    if (buf == NULL) return NULL_ARG;
    if (_checkNumeric(buf)) {
        *ptr = atof(buf);
    } else {
        return CONVERSION_ERROR;
    }
    
    return NO_ERROR;
}

int CommandData::getNext(long* ptr) {
    char* buf = getNext();
    if (buf == NULL) return NULL_ARG;
    if (_checkNumeric(buf)) {
        *ptr = atol(buf);
    } else {
        return CONVERSION_ERROR;
    }
    
    return NO_ERROR;
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