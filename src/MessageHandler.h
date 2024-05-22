#pragma once
#include <Arduino.h>
#include "RingBuffer.h"

class MessageHandlerBase {
    protected:
        MessageHandlerBase() {}

    public:
        virtual int available();

        virtual void printMessage(const char* format, ...);
        virtual void printMessage(const char* level, const char* format, ...);

        virtual void queueMessage(const char* format, ...);
        virtual void queueMessage(const char* level, const char* format, ...);

};


template<uint8_t MESSAGE_QUEUE_SIZE = 5, uint8_t MESSAGE_BUFFER_SIZE = 64>
class MessageHandler: public MessageHandlerBase {
    private:
        
        struct Message {
            char buffer[MESSAGE_BUFFER_SIZE];
        
            Message() {};

            void build(const char* format, va_list args) {
                vsnprintf(buffer, MESSAGE_BUFFER_SIZE, format, args);
            }

            void build(const char* level, const char* format, va_list args) {
                snprintf(buffer, MESSAGE_BUFFER_SIZE, "[%s] ", level);
                vsnprintf(buffer + strlen(buffer), MESSAGE_BUFFER_SIZE, format, args);
            }

            void print(Stream* serial) {
                serial->println(buffer);
            }
            
        };

        Stream* serialStream;
        RingBuffer<Message, MESSAGE_QUEUE_SIZE> messageQueue;  

    public:
        template<uint8_t, uint8_t, uint8_t> friend class CommandHandler;

        MessageHandler(Stream* stream) {
            serialStream = stream;
        }

        void printMessage(const char* format, ...) {
            va_list args;
            Message message;

            va_start(args, format);
            message.build(format, args);
            va_end(args);

            message.print(serialStream);
        }

        void printMessage(const char* level, const char* format, ...) {
            va_list args;
            Message message;

            va_start(args, format);
            message.build(level, format, args);
            va_end(args);

            message.print(serialStream);
        }

        void queueMessage(const char* format, ...) {
            va_list args;
            Message message;

            va_start(args, format);
            message.build(format, args);
            va_end(args);

            messageQueue.put(message);
        }

        void queueMessage(const char* level, const char* format, ...) {
            va_list args;
            Message message;

            va_start(args, format);
            message.build(level, format, args);
            va_end(args);

            messageQueue.put(message);
        }

        int available() {
            return serialStream->available();
        }
};