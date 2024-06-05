#pragma once

#include <Arduino.h>
#include "Config.h"

#define CALLBACK(name) void (*name)(SerialCommander*)

typedef void (*CallbackFunction)(SerialCommander*);


class SerialCommander {

    private:

        struct _command {
            char name[COMMAND_NAME_MAX_LEN + 1];
            CALLBACK(callback);
        };

        CALLBACK(_default);

        uint8_t _numCmds;
        _command _cmds[MAX_COMMANDS];

        Stream* _serial;

        char* _cmdName;
        const char _argDelim, _cmdDelim, _cmdStart, _cmdEnd;


        void _addCommand(_command* command, CALLBACK(callback));

        void _pollSerial();
        void _parseSerial(char* data);

        void _resetIndexer();

    public:

        SerialCommander(Stream* serial, const char argDelim, const char cmdDelim, const char cmdStart, const char cmdEnd);
        SerialCommander(Stream* serial, const char argDelim, const char cmdDelim);


        void addCommand(const char* name, CALLBACK(callback));

        void run();
};