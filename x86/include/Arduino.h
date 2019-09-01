#ifndef __ARDUINO_H
#define __ARDUINO_H
#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include <unistd.h>

#define A0                (0xFF)

#define OUTPUT            (0x0)
#define INPUT             (0x1)
#define INPUT_PULLUP      (0x2)

#define HIGH              (0x1)
#define LOW               (0x0)

#define F(x_)		x_

#ifdef __cplusplus
#include <string> 

// using namespace std;

class String : public std::string {
public:
  String(void) : std::string()
  {}
  String(const char* str) : std::string(str)
  {}
  String(int32_t value) : std::string(std::to_string(value))
  {}

  //void concat(const char* str) {}
  void concat(String str) { }
private:

};

class CSerial {
public:
  CSerial(void)   {}

  void begin(void)                     {}
  void print(const char* str)          { printf("%s", str); }
  void print(int32_t v)                { printf("%d", v); }
  void println(const char* str)        { printf("%s\n", str); }

private:

};

static CSerial Serial;

extern "C" {
#endif

static inline
void pinMode(const uint8_t pin, const uint8_t mode) { }

static inline
uint16_t analogRead(const uint8_t pin) {
  printf("    ## analogRead(%u)  : 0\n", pin); return 0;
}

static inline
uint16_t digitalRead(const uint8_t pin) {
  printf("    ## digitalRead(%u) : 0\n", pin); return 0;
}

static inline
void digitalWrite(const uint8_t pin, const uint8_t value) {
  printf("    ## digitalWrite(%u): %u\n", pin, value);
}

static inline
uint32_t millis(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (uint32_t)(ts.tv_sec*1000 + (ts.tv_nsec/1000000UL));
}

static inline
uint32_t micros(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (uint32_t)(ts.tv_sec*1000000 + (ts.tv_nsec/1000UL));
}

static inline
void delay(const uint32_t delay_ms) {
  usleep(delay_ms * 1000);
}

static inline
void delayMicroseconds(const uint32_t delay_us) {
  usleep(delay_us);
}

#ifdef __cplusplus
}
#endif

#endif      /*__ARDUINO_H*/
