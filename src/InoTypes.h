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

#define INO_DECLARE_STATIC      static inline
#define INO_STATIC              static

#define INO_NOP                 /* INO_NOP */
#define INO_WRAP_FUNC(fn_)      do { fn_ } while (0);

#define INO_TO_CSTRING(string)  (string.c_str())

#define RTIME_FMT               "%u"
#define RTIME_DIFF_FMT          "%d"

#define INO_CR                  "\r\n"

#define INO_UNUSED(x_)          (void)(x_);

#define INO_MIN(x, y) \
  (((x)<(y)) ? (x) : (y))
#define INO_MAX(x, y) \
  (((x)>(y)) ? (x) : (y))
#define INO_CLAMP(value, min, max) \
  INO_MAX(min, INO_MIN(max, value))

INO_NAMESPACE

typedef uint32_t        rtime_t;
typedef int32_t         rtime_diff_t;
typedef void*           handle;

INO_NAMESPACE_END

#endif    /*__INO_TYPES_H__*/
