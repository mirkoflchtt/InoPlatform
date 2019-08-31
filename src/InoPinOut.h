#ifndef __INO_PIN_OUT__
#define __INO_PIN_OUT__
#include "InoTypes.h"
#include "InoHalAdapter.h"

INO_NAMESPACE

class PinOut {
public:
  PinOut(
    const uint8_t pin, const bool high_=false, const bool swap_high_low_=false);

  bool      swapHighLow(void);

  bool      high(void);
  bool      blink(void);
  
  void      high(const bool high_);
  void      blink(const uint32_t msec_delay, const uint8_t loops=1);
  void      toggle(void);
  

private:
  bool        m_swap_high_low;
  uint16_t    m_state;
  clock_ts    m_start;

};

INO_NAMESPACE_END

#endif    /*__INO_PIN_OUT__*/
