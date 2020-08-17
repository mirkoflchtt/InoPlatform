#ifndef __ARDUINO_TICKER_H
#define __ARDUINO_TICKER_H

#include "Arduino.h"

typedef void(*   callback_t) (void);
typedef void(*   arg_callback_t) (void*);

#ifdef __cplusplus

class Ticker {
public:
  Ticker(void) :
    rearm(false), interval(0), ts(0), cb(NULL)
  {}

  void trigger(const uint32_t _now)
  {
    if (cb && (interval>0) && (_now+interval)>=ts) {
      cb();
      ts = _now;
      if ( !rearm ) detach();
    }
  }

  void  attach(float seconds, callback_t callback) {
    attach_ms((uint32_t)(seconds * 1000 + 0.5f), callback);
  }
 
  void  attach_ms(uint32_t milliseconds, callback_t callback)
  {
    rearm     = true;
    ts        = millis();
    interval  = milliseconds;
    cb        = callback;
    cb_cookie = NULL;
  }

  void  attach(float seconds, arg_callback_t callback, void* cookie=NULL) {
    attach_ms((uint32_t)(seconds * 1000 + 0.5f), callback, cookie);
  }
 
  void  attach_ms(uint32_t milliseconds, arg_callback_t callback, void* cookie=NULL)
  {
    rearm     = true;
    ts        = millis();
    interval  = milliseconds;
    arg_cb    = callback;
    cb_cookie = cookie;
  }

  void  once(float seconds, callback_t callback) {
    once_ms((uint32_t)(seconds * 1000 + 0.5f), callback);
  }
 
  void  once_ms(uint32_t milliseconds, callback_t callback)
  {
    attach_ms(milliseconds, callback);
    rearm    = false;
  }

  void detach(void) {
    interval = 0;
  }

private:
  bool       rearm;
  uint32_t   interval;
  uint32_t   ts;
  union {
    callback_t      cb;
    arg_callback_t  arg_cb;
  };
  void*      cb_cookie;
};

extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif             /*__ARDUINO_TICKER_H*/
