#ifndef INO_TYPES_H
#define INO_TYPES_H
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

#define INO_MALLOC(count, type) malloc((count)*sizeof(type))
#define INO_FREE(ptr)           { if (ptr) free(ptr); ptr = NULL; }

#define INO_NOP                 /* INO_NOP */
#define INO_WRAP_FUNC(fn)       do { fn } while (0);

#define INO_TO_CSTRING(string)  (string.c_str())

#define RTIME_FMT               "%u"
#define RTIME_DIFF_FMT          "%d"

#define INO_CR                  "\r\n"

#define INO_UNUSED(x) \
  (void)(x);

#define INO_MIN(x, y) \
  (((x)<(y)) ? (x) : (y))

#define INO_MAX(x, y) \
  (((x)>(y)) ? (x) : (y))

#define INO_CLAMP(value, min, max) \
  INO_MAX(min, INO_MIN(max, value))

#define INO_ROUND(value) \
  ((ino_i32)(((value)<0) ? ((value) + 0.5f) : ((value) - 0.5f)))


#define INO_HANDLE_NULL         INO_HANDLE(0)
#define INO_HANDLE(ptr)         ((ino_handle)(ptr))
#define INO_PTR(ptr)            ((ino_ptr)(ptr))
#define INO_PTR_CONST(ptr)      ((ino_ptr_const)(ptr))
#define INO_UPTR(ptr)           ((ino_uptr)(ptr))

/******************************************************************************/
#define INO_PACK(...) \
  __VA_ARGS__

/******************************************************************************/
#define INO_ARRAY_COUNT(array) \
  ( sizeof(array)/sizeof((array)[0]) )

#define INO_ARRAY_STATIC(size, type, ...) \
  (type[size]) { __VA_ARGS__ } 

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
typedef uint32_t        ino_size;
typedef char            ino_char;

typedef int             ino_int;
typedef int8_t          ino_i8;
typedef int16_t         ino_i16;
typedef int32_t         ino_i32;
typedef int64_t         ino_i64;

typedef unsigned int    ino_uint;
typedef uint8_t         ino_u8;
typedef uint16_t        ino_u16;
typedef uint32_t        ino_u32;
typedef uint64_t        ino_u64;

typedef int32_t         ino_diff_ts;

typedef int32_t         ino_error;

typedef uint32_t        ino_timestamp;
typedef uint32_t        ino_interval;

INO_NAMESPACE

INO_NAMESPACE_END

#endif    /*INO_TYPES_H*/
