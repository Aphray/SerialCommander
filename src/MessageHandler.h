#pragma once
#include <Arduino.h>
#include "RingBuffer.h"

class MessageBase {
    public:

};

template<uint8_t BUFFER_SIZE = 64>
class Message {
    private:
        char buffer[BUFFER_SIZE];
    
    public:
        Message() {};

        // template<typename... Args>
        // void build(char* format, Args... args) {
        //     sprintf(buffer + strlen(buffer), format, args...);
        // }

        // template<typename... Args>
        // void build(char* level, char* format, Args... args) {
        //     sprintf(buffer, "[%s] ", level);
        //     sprintf(buffer + strlen(buffer), format, args...);
        // }

        // template<typename... Args>
        // void build(const __FlashStringHelper* format, Args... args) {
        //     sprintf_P(buffer + strlen(buffer), (PGM_P) format, args...);
        // }

        // template<typename... Args>
        // void build(const __FlashStringHelper* level, const __FlashStringHelper* format, Args... args) {
        //     sprintf_P(buffer, "[%s] ", (PGM_P) level);
        //     sprintf_P(buffer + strlen(buffer), (PGM_P) format, args...);

        //     vsprintf
        // }

        void build(const char* format, va_list args) {
            vsnprintf(buffer, BUFFER_SIZE, format, args);
        }

        void build(const char* level, const char* format, va_list args) {
            snprintf(buffer, BUFFER_SIZE, "[%s] ", level);
            vsnprintf(buffer + strlen(buffer), BUFFER_SIZE, format, args);
        }

        void print(Stream* serial) {
            serial->println(buffer);
        }
      
};

class MessageHandlerBase {
    protected:
        MessageHandlerBase() {}

    public:
        virtual void printMessage(const char* format, ...);
        virtual void printMessage(const char* level, const char* format, ...);

        virtual void queueMessage(const char* format, ...);
        virtual void queueMessage(const char* level, const char* format, ...);

};


template<uint8_t MESSAGE_QUEUE_SIZE = 5, uint8_t MESSAGE_BUFFER_SIZE = 64>
class MessageHandler: public MessageHandlerBase {
    private:
        Stream* serialStream;
        RingBuffer<Message<MESSAGE_BUFFER_SIZE>, MESSAGE_QUEUE_SIZE> messageQueue;

    public:

        MessageHandler(): MessageHandlerBase() {}

        void printMessage(const char* format, ...) {
            va_list args;
            Message<MESSAGE_BUFFER_SIZE> message;

            va_start(args, format);
            message.build(format, args);
            va_end(args);

            message.print(serialStream);
        }

        void printMessage(const char* level, const char* format, ...) {
            va_list args;
            Message<MESSAGE_BUFFER_SIZE> message;

            va_start(args, format);
            message.build(level, format, args);
            va_end(args);

            message.print(serialStream);
        }

        void queueMessage(const char* format, ...) {
            va_list args;
            Message<MESSAGE_BUFFER_SIZE> message;

            va_start(args, format);
            message.build(format, args);
            va_end(args);

            messageQueue.put(message);
        }

        void queueMessage(const char* level, const char* format, ...) {
            va_list args;
            Message<MESSAGE_BUFFER_SIZE> message;

            va_start(args, format);
            message.build(level, format, args);
            va_end(args);

            messageQueue.put(message);
        }
};