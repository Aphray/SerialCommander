#include <Arduino.h>
#include <assert.h>

#define RX_BUFFER_SIZE 128
#define TX_BUFFER_SIZE 256

#include "CommandHandler.h"

// template <typename T> struct TypeInfo { static constexpr char* name = "n/a"; };

// #define MAKE_TYPE_INFO(type) template<> struct TypeInfo<type> { static constexpr char* name = #type; }

// MAKE_TYPE_INFO(int);
// MAKE_TYPE_INFO(bool);
// MAKE_TYPE_INFO(char*);
// MAKE_TYPE_INFO(const char*);
// MAKE_TYPE_INFO(const __FlashStringHelper*);

// constexpr bool strequal(char const * a, char const * b) {
//     return *a == *b && (*a == '\0' || strequal(a + 1, b + 1));
// }

// #define ASSERT_TYPE(var, type) static_assert(strequal(TypeInfo<typeof(var)>::name, #type), "Wrong type!")

// #define TEST F("test")

template <typename T>
T func(const char* b) {

}

template <typename T>
void foo(T (*f)(const char*)) {

};

void setup() {

  foo(&atoi);

  // ASSERT_TYPE(TEST, const __FlashStringHelper*);
  Serial.begin(9600);

}

void loop() {
  Serial.println(foo());
  // Serial.println(RX_BUFFER_SIZE);
  // Serial.println(TX_BUFFER_SIZE);
}