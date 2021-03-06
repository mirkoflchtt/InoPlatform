#ifndef INO_LOG_H
#define INO_LOG_H
#ifndef HAS_X86
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#endif
#include <stdarg.h>

#include "InoMyConfig.h"
#include "InoTypes.h"

#ifdef HAS_X86
#include <stdio.h>
class PubSubClient {
public:
  bool connected(void) { return true; }
  void publish(const char* topic, const char* msg) {
    printf("topic: %s msg: %s\n", topic, msg);
  }
};

class Stream {
public:
  void printf(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
  }
};

#endif  /*HAS_X86*/

#define INO_LOG_VERSION "1.0.0"

/******************************************************************************/
//#define INO_LOG_USE_COLOR
//#define INO_LOG_HAS_FILE_OUT
//#define INO_LOG_HAS_DATETIME

#define INO_LOG_STRIP_FILE_PATH     '/'
//#define LOG_STRIP_FILE_PATH     '\\'


typedef ino_u8 LogLevel;

#define LOG_SUDO            (0x0)
#define LOG_FATAL           (0x1)
#define LOG_ERROR           (0x2)
#define LOG_WARN            (0x3)
#define LOG_INFO            (0x4)
#define LOG_DEBUG           (0x5)
#define LOG_TRACE           (0x6)

//#define INO_HAS_LOG         LOG_TRACE

#define INO_NO_LOG          /* INO_NO_LOG */

#if (defined INO_HAS_LOG && (INO_HAS_LOG>=LOG_SUDO))
  #define INO_LOG_SUDO(...) \
    { ino::logMsg(LOG_SUDO,  __FILE__, __LINE__, __VA_ARGS__); }
#else
  #define INO_LOG_SUDO(...)  INO_NO_LOG
#endif
#if (defined INO_HAS_LOG && (INO_HAS_LOG>=LOG_FATAL))
  #define INO_LOG_FATAL(...) \
    { ino::logMsg(LOG_FATAL,  __FILE__, __LINE__, __VA_ARGS__); }
#else
  #define INO_LOG_FATAL(...)  INO_NO_LOG
#endif
#if (defined INO_HAS_LOG && (INO_HAS_LOG>=LOG_ERROR))
  #define INO_LOG_ERROR(...) \
    { ino::logMsg(LOG_ERROR,  __FILE__, __LINE__, __VA_ARGS__); }
#else
  #define INO_LOG_ERROR(...)  INO_NO_LOG
#endif
#if (defined INO_HAS_LOG && (INO_HAS_LOG>=LOG_WARN))
  #define INO_LOG_WARN(...) \
    { ino::logMsg(LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__); }
#else
  #define INO_LOG_WARN(...)  INO_NO_LOG
#endif
#if (defined INO_HAS_LOG && (INO_HAS_LOG>=LOG_INFO))
  #define INO_LOG_INFO(...) \
    { ino::logMsg(LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__); }
#else
  #define INO_LOG_INFO(...)  INO_NO_LOG
#endif
#if (defined INO_HAS_LOG && (INO_HAS_LOG>=LOG_DEBUG))
  #define INO_LOG_DEBUG(...) \
    { ino::logMsg(LOG_DEBUG,  __FILE__, __LINE__, __VA_ARGS__); }
#else
  #define INO_LOG_DEBUG(...)  INO_NO_LOG
#endif
#if (defined INO_HAS_LOG && (INO_HAS_LOG>=LOG_TRACE))
  #define INO_LOG_TRACE(...) \
    { ino::logMsg(LOG_TRACE,  __FILE__, __LINE__, __VA_ARGS__); }
#else
  #define INO_LOG_TRACE(...)  INO_NO_LOG
#endif

INO_NAMESPACE

INO_API_DECLARE


void logSetName(
  const char* name);

void logSetQuiet(
  const ino_bool quiet);

void logSetLevel(
  const LogLevel level);

void logSetMqtt(
  PubSubClient* mqtt, const char* mqtt_topic);

void logSetStream(
  Stream* stream);

ino_u32 logMsg(
  const LogLevel level,
  const char *file, const int line,
  const char* fmt, ...);


INO_API_DECLARE_END

INO_NAMESPACE_END

#endif   /*INO_LOG_H*/
