#ifndef __ARDUINO_TICKER_H
#define __ARDUINO_TICKER_H

#include "Arduino.h"

typedef void (*tickerCB)(void);

#ifdef __cplusplus

class Ticker {
public:
  Ticker(void) :
    interval(0), ts(0), cb(NULL)
  {}

  void trigger(const uint32_t _now)
  {
    if (cb && (_now+interval)>=ts) { cb(); ts = _now; }
  }

  void attach(const uint32_t _interval, tickerCB _cb)
  {
    interval = _interval; cb = _cb;
  }
  
private:
  uint32_t   interval;
  uint32_t   ts;
  tickerCB   cb;
};

extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif             /*__ARDUINO_TICKER_H*/
