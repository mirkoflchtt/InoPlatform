#ifndef __INO_TYPES_H__
#define __INO_TYPES_H__
#include <stdint.h>
#include <stdbool.h>

//#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
//#else
//#include <WProgram.h>
//#endif

#include "InoMyConfig.h"

#ifdef __cplusplus
  #define INO_API_DECLARE       extern "C" {
  #define INO_API_DECLARE_END   }
  #define INO_NAMESPACE         namespace ino {
  #define INO_NAMESPACE_END     }
#else
  #define INO_API_DECLARE       /* INO_API_DECLARE */
  #define INO_API_DECLARE_END   /* INO_API_DECLARE_END */
  #define INO_NAMESPACE         /* INO_NAMESPACE */
  #define INO_NAMESPACE_END     /* INO_NAMESPACE_END */
#endif

#define INO_UNUSED_FUNCTION     /* INO_UNUSED_FUNCTION */
#define INO_API_ENTRY           /* INO_API_ENTRY */
#define INO_DECLARE_STATIC      static inline
#define INO_STATIC              static

#define INO_NOP                 /* INO_NOP */
#define INO_WRAP_FUNC(fn_)      do { fn_ } while (0);

#define INO_TO_CSTRING(string)  (string.c_str())

#define RTIME_FMT               "%u"
#define RTIME_DIFF_FMT          "%d"

#define INO_CR                  "\r\n"

#define INO_UNUSED(x_) \
  (void)(x_);

#define INO_MIN(x, y) \
  (((x)<(y)) ? (x) : (y))

#define INO_MAX(x, y) \
  (((x)>(y)) ? (x) : (y))

#define INO_CLAMP(value, min, max) \
  INO_MAX(min, INO_MIN(max, value))

#define INO_HANDLE_NULL         INO_HANDLE(0)
#define INO_HANDLE(ptr)         ((ino_handle)(ptr))
#define INO_PTR(ptr)            ((ino_ptr)(ptr))

/******************************************************************************/
#define INO_ARRAY_COUNT(array_) \
  ( sizeof(array_)/sizeof((array_)[0]) )

#define INO_STATIC_TYPE_ARRAY_DECLARE(type_, size_, ...) \
  (type_[size_]) { ##__VA_ARGS__ } 

/******************************************************************************/
#define INO_COLOR_BLACK            "\x1b[30m"
#define INO_COLOR_RED              "\x1b[31m"
#define INO_COLOR_GREEN            "\x1b[32m"
#define INO_COLOR_YELLOW           "\x1b[33m"
#define INO_COLOR_BLUE             "\x1b[94m"
#define INO_COLOR_MAGENTA          "\x1b[35m"
#define INO_COLOR_CYAN             "\x1b[36m"
#define INO_COLOR_WHYTE            "\x1b[37m"
#define INO_COLOR_DEFAULT          "\x1b[39m"
#define INO_COLOR_LGRAY            "\x1b[90m"
#define INO_COLOR_LRED             "\x1b[91m"
#define INO_COLOR_LGREEN           "\x1b[92m"
#define INO_COLOR_LYELLOW          "\x1b[93m"
#define INO_COLOR_LBLUE            "\x1b[94m"
#define INO_COLOR_LMAGENTA         "\x1b[95m"
#define INO_COLOR_LCYAN            "\x1b[96m"
#define INO_COLOR_LWHITE           "\x1b[97m"
#define INO_COLOR_OFF              "\x1b[0m"

/******************************************************************************/
typedef void*           ino_handle;
typedef uint8_t*        ino_ptr;
typedef const uint8_t*  ino_ptr_const;
typedef uintptr_t       ino_uptr;

typedef bool            ino_bool;
typedef float           ino_float;

typedef int8_t          ino_i8;
typedef int16_t         ino_i16;
typedef int32_t         ino_i32;
typedef int64_t         ino_i64;

typedef uint8_t         ino_u8;
typedef uint16_t        ino_u16;
typedef uint32_t        ino_u32;
typedef uint64_t        ino_u64;

INO_NAMESPACE

typedef uint32_t        rtime_t;
typedef int32_t         rtime_diff_t;
typedef void*           handle;

INO_NAMESPACE_END

#endif    /*__INO_TYPES_H__*/
