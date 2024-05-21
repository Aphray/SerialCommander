#include "ArgList.h"

ArgList::ArgList(char* args, const char delimiter): delimiter(delimiter) {
    uint8_t n = 0;
    while (args[n] != 0) {
        if (args[n] == delimiter) numArgs++;
        n++;
    }
    numArgs++;
    this->args = args;
    resetIndex();
}

void ArgList::resetIndex() {
    savePtr = &args;
}


Arg<char*> ArgList::next() {
    Arg<char*> ret;
    ret.value = strtok_r(args, &delimiter, savePtr);
    if (ret.value == NULL) ret.error = true;
    return ret;
}

Arg<int> ArgList::nextToInt() {
    Arg<char*> arg = next();

    Arg<int> ret {0, arg.error};
    if (ret.error) return ret;
}