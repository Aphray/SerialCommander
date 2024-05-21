#pragma once

#include <Arduino.h>


template<typename T, uint16_t SIZE>
class RingBuffer {
    private:
        uint16_t tail;
        uint16_t head;
        uint16_t count;

        T queue[SIZE];
        
    public:
        RingBuffer() {
            tail = 0;
            head = 0;
            count = 0;
        }

        inline bool isFull() { return count == SIZE; }
        inline bool isEmpty() { return count == 0; }

        inline uint16_t getMaxSize() { return SIZE; }
        inline uint16_t getFreeSpace() { return SIZE - count; }
        inline uint16_t getCurrentSize() { return count; }

        inline void clear() {
            count = 0;
            tail = head = 0;
        }
        
        T* pop() {
            if (isEmpty()) return nullptr;
            T* result = &(queue[tail]);
            count--;
            tail = (tail + 1) % SIZE;
            return result;
        }

        bool put(const T& item) {
            if (isFull()) return false;
            queue[head] = item;
            count++;
            head = (head + 1) % SIZE;
            return true;
        }
};