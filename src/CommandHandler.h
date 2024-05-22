#pragma once

#include <Arduino.h>
#include "MessageHandler.h"
#include "ArgList.h"


#define MAKE_CALLBACK(fname) void fname(char* name, ArgList* args)


template<uint8_t MAX_COMMANDS = 10, uint8_t MAX_COMMAND_CALLBACKS = 3, uint8_t RX_BUFFER_SIZE = 64>
class CommandHandler {

    private:

        struct CommandCallback {
            int8_t priority;
            void (*function)(char*, ArgList*);
        };

        struct Command {
            char* name;
            uint8_t numCallbacks;
            CommandCallback callbacks[MAX_COMMAND_CALLBACKS];
        };


        uint8_t numCommands;
        Command commands[MAX_COMMANDS];

        const char argDelimiter;
        const char cmdDelimiter;
        const char cmdStart;
        const char cmdEnd;

        bool debugMode;

        MessageHandlerBase* messageHandler;

        void addCommand(Command* command, void (*function)(char*, ArgList*), int8_t priority) {
            if (command->numCallbacks == MAX_COMMAND_CALLBACKS) {
                messageHandler->printMessage("ERROR", "Max callbacks reached for <%s>", command->name);
                return;
            }

            CommandCallback* newCallback = &(command->callbacks[command->numCallbacks++]);
            newCallback->priority = priority;
            newCallback->function = function;
        }

        void runCommand(Command* command, ArgList* args) {
            for (CommandCallback callback: command->callbacks) {
                callback.function(command->name, args);
            }
        }

        void pollSerial() {
            static char rxBuffer[RX_BUFFER_SIZE];
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
            ArgList args(strtok(NULL, &cmdDelimiter), cmdDelimiter);

            for (Command command: commands) {
                if (strcmp(command.name, name) == 0) {
                    args.resetIndex();
                    runCommand(&command, &args);
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

        void addCommandCallback(const char* name, void (*function)(char*, ArgList*), int8_t priority = 10) {
            for (Command command: commands) {
                if (strcmp(command.name, name) == 0) {
                    addCommand(&command, function, priority);
                    return;
                }
            }

            if (numCommands == MAX_COMMANDS) {
                messageHandler->printMessage("ERROR", "Max commands reached");
                return;
            }

            Command* newCommand = &(commands[numCommands++]);
            newCommand->name = name;
            addCommand(newCommand, function, priority);
        }

        void enableDebug() {
            debugMode = true;
        }

        void process() {
            pollSerial();
        }
};