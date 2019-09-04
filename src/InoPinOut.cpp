#include "InoPinOut.h"

#define PINOUT_PIN_MASK     (0x001F)
#define PINOUT_HIGH_MASK    (0x8000)
#define PINOUT_BLINK_MASK   (0x4000)

#define PINOUT_PIN(x)       (((x) & PINOUT_PIN_MASK)>>0)
#define PINOUT_HIGH(x)      (((x) & PINOUT_HIGH_MASK)>>15)
#define PINOUT_BLINK(x)     (((x) & PINOUT_BLINK_MASK)>>14)


INO_NAMESPACE

PinOut::PinOut(
  const uint8_t pin, const bool high_, const bool swap_high_low_) :
m_swap_high_low(swap_high_low_),
m_state(PINOUT_PIN(pin)),
m_start(clock_ms())
{
  pinMode(PINOUT_PIN(m_state), OUTPUT);
  high(high_);
}

bool PinOut::swapHighLow(void)
{
  m_swap_high_low = (m_swap_high_low) ? false : true;
  return m_swap_high_low;
}

bool PinOut::high(void)
{
  return PINOUT_HIGH(m_state) ? true : false;
}


bool PinOut::blink(void)
{
  return PINOUT_BLINK(m_state) ? true : false;
}

void PinOut::high(const bool high_)
{
  m_start     = clock_ms();
  m_state &= ~PINOUT_BLINK_MASK;
  
  if ( high_ ) {
    m_state |=   PINOUT_HIGH_MASK;
    digitalWrite(PINOUT_PIN(m_state), ((m_swap_high_low) ? LOW : HIGH));
  } else {
    m_state &=  ~PINOUT_HIGH_MASK;
    digitalWrite(PINOUT_PIN(m_state), ((m_swap_high_low) ? HIGH : LOW));
  }
}

void PinOut::blink(const uint32_t msec_delay, const uint8_t loops)
{
  for ( uint8_t i=0; i<loops; i++ ) {
    // If already in blink state
    if ( blink() ) {
      // switch the led after interval milliseconds elapsed 
      //if ( clock_ms()>=m_start+msec_delay ) {
      if ( trigger_event(clock_ms(), m_start, msec_delay) ) {
        toggle();
        m_state |= PINOUT_BLINK_MASK;
      }
    } else {
      // not in blink state
      // so switch the led and mark the state as blinking
      toggle();
      m_state |= PINOUT_BLINK_MASK;
    }
    if ( loops>1 ) {
      wait_ms(msec_delay);
    }
  }
}

void PinOut::toggle(void)
{
  high( !high() );
}

  
INO_NAMESPACE_END
