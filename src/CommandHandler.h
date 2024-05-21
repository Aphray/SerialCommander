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
            PGM_P name;
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

        bool compareStr(const char* a, const char* b) {
            return strcmp_P(a, b) == 0;
        }

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
            static bool cmdStarted = (cmdStart == 0) ? true : false;

            while (Serial.available() > 0) {
                char rx = Serial.read();

                if (rx == cmdStart) {
                    if (cmdStarted) goto finish;
                    cmdStarted = true;
                } else if (rx == cmdEnd) {
                    if (cmdStarted) parseSerial(rxBuffer);
                    goto finish;
                } else if (rx != -1 && rx != '\r' && rx != '\n') {
                    *ptr++ = rx;
                }
            }

            finish:
                rxBuffer[0] = 0;
                ptr = rxBuffer;
                cmdStarted = (cmdStart == 0) ? true : false;
        }

        void parseSerial(char* data) {
            // data is empty, nothing to do...
            if (data[0] == 0) return;

            char* name = strtok(data, &cmdDelimiter);
            ArgList args(strtok(NULL, &cmdDelimiter));

            for (Command command: commands) {
                if (STR_CMP_P(command.name, name)) {
                    args.resetIndex();
                    runCommand(&command, &args);
                    return;
                }
            }

            messageHandler->printMessage("ERROR", "Cmd <%s> unrecognized", name);
        }
    
    public:
        CommandHandler( MessageHandlerBase* messageHandler, const char argDelimiter, const char cmdDelimiter, const char cmdStart = 0, const char cmdEnd = '\n',): 
            argDelimiter(argDelimiter), cmdDelimiter(cmdDelimiter), cmdStart(cmdStart), cmdEnd(cmdEnd) {
                debugMode = false;
                this->messageHandler = messageHandler;
            };

        void addCommandCallback(const __FlashStringHelper* name, void (*function)(char*, ArgList*), int8_t priority = 10) {
            PGM_P name_p = (PGM_P) name;
            for (Command command: commands) {
                if (compareStr(command.name, name_p)) {
                    addCommand(&command, function, priority);
                    return;
                }
            }

            if (numCommands == MAX_COMMANDS) {
                messageHandler->printMessage("ERROR", "Max commands reached");
                return;
            }

            Command* newCommand = &(commands[numCommands++]);
            newCommand->name = name_p;
            addCommand(newCommand, function, priority);
        }

        void enableDebug() {
            debugMode = true;
        }

        void process() {
            pollSerial();
        }
};