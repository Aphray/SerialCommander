#pragma once

#include <Arduino.h>
#include "MessageHandler.h"
#include "CommandData.h"


#define MAKE_CALLBACK(fname) void fname(char* name, CommandData* args)


template<uint8_t CMD_CHAR_MAX = 10, uint8_t MAX_CMDS = 10, uint8_t MAX_CMD_CBS = 3, uint8_t RX_BUF_SIZE = 64>
class CommandHandler {

    private:

        struct CommandCallback {
            int8_t priority;
            void (*function)(char*, CommandData*);
        };

        struct Command {
            char name[CMD_CHAR_MAX + 1];
            uint8_t numCallbacks;
            CommandCallback callbacks[MAX_CMD_CBS];
        };


        uint8_t numCommands;
        Command commands[MAX_CMDS];

        const char argDelimiter;
        const char cmdDelimiter;
        const char cmdStart;
        const char cmdEnd;

        bool debugMode;

        MessageHandlerBase* messageHandler;

        void addCallback(Command* command, void (*function)(char*, CommandData*), int8_t priority) {
            if (command->numCallbacks == MAX_CMD_CBS) {
                messageHandler->printMessage("ERROR", "Max callbacks reached for <%s>", command->name);
                return;
            }

            CommandCallback* newCallback = &(command->callbacks[command->numCallbacks++]);
            newCallback->priority = priority;
            newCallback->function = function;
        }

        void runCallbacks(Command* command, CommandData* args) {
            for (CommandCallback callback: command->callbacks) {
                callback.function(command->name, args);
            }
        }

        void pollSerial() {
            static char rxBuffer[RX_BUF_SIZE];
            static char* ptr = rxBuffer;
            static bool started = (cmdStart == 0) ? true : false;

            bool cleanup = false;

            while (messageHandler->available() > 0) {
                char rx = Serial.read();

                if (rx == cmdStart) {
                    if (started) {
                        cleanup = true;
                        break;
                    }
                    started = true;
                } else if (rx == cmdEnd) {
                    if (started) {
                        *ptr = 0;
                        parseSerial(rxBuffer);
                    }
                    cleanup = true;
                } else if (rx != -1 && rx != '\r' && rx != '\n') {
                    *ptr++ = rx;
                }
            }

            if (cleanup) {
                rxBuffer[0] = 0;
                ptr = rxBuffer;
                started = (cmdStart == 0) ? true : false;
            }
        }

        void parseSerial(char* data) {
            // data is empty, nothing to do...

            if (data[0] == 0) return;

            char* name = strtok(data, &cmdDelimiter);
            CommandData args(strtok(NULL, &cmdDelimiter), cmdDelimiter);

            for (Command command: commands) {
                if (strcmp(command.name, name) == 0) {
                    args.resetIndex();
                    runCallbacks(&command, &args);
                    return;
                }
            }

            messageHandler->printMessage("ERROR", "Cmd <%s> unrecognized", name);
        }
    
    public:
        CommandHandler( MessageHandlerBase* messageHandler, const char argDelimiter, const char cmdDelimiter, const char cmdStart = 0, const char cmdEnd = '\n'): 
            argDelimiter(argDelimiter), cmdDelimiter(cmdDelimiter), cmdStart(cmdStart), cmdEnd(cmdEnd) {
                debugMode = false;
                this->messageHandler = messageHandler;
            };


        void addCommandCallback(const __FlashStringHelper* name, void (*function)(char*, CommandData*)) {
            char buf[CMD_CHAR_MAX] = {0};
            strncpy_P(buf, (PGM_P) name, CMD_CHAR_MAX);

        }

        void addCommandCallback(const char* name, void (*function)(char*, CommandData*), int8_t priority = 10) {
            for (Command command: commands) {
                if (strcmp(command.name, name) == 0) {
                    addCallback(&command, function, priority);
                    return;
                }
            }

            if (numCommands == MAX_CMDS) {
                messageHandler->printMessage("ERROR", "Max commands reached");
                return;
            }


            Command* newCommand = &(commands[numCommands++]);
            // newCommand->name = name;
            newCommand->name[0] = 0;
            strncpy(newCommand->name, name, CMD_CHAR_MAX);
            addCallback(newCommand, function, priority);
        }

        void enableDebug() {
            debugMode = true;
        }

        void process() {
            pollSerial();
        }
};