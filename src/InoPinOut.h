#ifndef __INO_PIN_OUT
#define __INO_PIN_OUT
#include "InoTypes.h"
#include "InoHalAdapter.h"

INO_NAMESPACE

class PinOut {
public:
  PinOut(
    const ino_u8 pin,
    const ino_bool high_=false,
    const ino_bool swap_high_low_=false);

  ino_bool      swapHighLow(void);

  ino_bool      high(void);
  ino_bool      blink(void);
  
  void      high(const ino_bool high_);
  void      blink(const ino_u32 msec_delay, const ino_u8 loops=1);
  void      toggle(void);
  
private:
  ino_bool    m_swap_high_low;
  ino_u16     m_state;
  clock_ts    m_start;
};

INO_NAMESPACE_END

#endif    /*__INO_PIN_OUT*/
