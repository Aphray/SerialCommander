#include <Arduino.h>
#include "SerialCommander.h"

SerialCommander::SerialCommander(Stream* serial, const char argDelim, const char cmdDelim, const char cmdStart, const char cmdEnd):
    _argDelim(argDelim), _cmdDelim(cmdDelim), _cmdStart(cmdStart), _cmdEnd(cmdEnd) {

        _serial = serial;
        _default = nullptr;
    }

SerialCommander::SerialCommander(Stream* serial, const char argDelim, const char cmdDelim): 
    SerialCommander(serial, argDelim, cmdDelim, 0, 0) {};


void SerialCommander::_pollSerial() {
    static char rxBuf[RX_BUFFER_SIZE + 1];
    static char* rxPtr = rxBuf;
    static bool started = (_cmdStart == 0) ? true : false;

    bool cleanup = false;

    while (_serial->available() > 0) {
        char rx = _serial->read();

        if (rx == _cmdStart && started) {
            if (started) {
                cleanup = true;
                break;
            }
            started = true;
        } else if (rx == _cmdEnd) {
            if (started) {
                *rxPtr = 0;
                _parseSerial(rxBuf);
            }
            cleanup = true;
        } else if (rx != -1 && rx != '\r' && rx != '\n') *rxPtr++ = rx;
    }

    if (cleanup) {
        rxBuf[0] = 0;
        rxPtr = rxBuf;
        started = (_cmdStart == 0) ? true : false;
    }
}

void SerialCommander::_parseSerial(char* data) {
    if (data[0] == 0) return;

    _cmdName = strtok(data, &_cmdDelim);

    for (_command cmd: _cmds) {
        if (strcmp(cmd.name, _cmdName) == 0) {
            _resetIndexer();
            (*cmd.callback)(this);
            return;
        }
    }

    if (_default != nullptr) _default(this);

}


void SerialCommander::addCommand(const char* name, CALLBACK(callback)) {
    for (_command cmd: _cmds) {
        if (strcmp(cmd.name, name) == 0)
            return;
    }

    if (_numCmds == MAX_COMMANDS)
        return;

    _command* newCmd = &(_cmds[_numCmds++]);
    newCmd->callback = callback;
    newCmd->name[0] = 0;
    strncpy(newCmd->name, name, COMMAND_NAME_MAX_LEN);
}