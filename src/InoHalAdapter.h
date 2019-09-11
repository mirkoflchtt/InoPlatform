#ifndef __INO_HAL_ADAPTER_H__
#define __INO_HAL_ADAPTER_H__
#include <Arduino.h>
#include "InoTypes.h"

#define CLOCK_MAX_VALUE     (~((clock_ts)0))

//#define CLOCK_FMT       "0x%08x"
#define CLOCK_FMT       "%u"
#define DELAY_FMT       "%u"

INO_NAMESPACE

typedef uint32_t        clock_ts;
typedef uint32_t        delay_ts;


INO_API_DECLARE

/***** functions for enabling and disabling interrupts ************************/

static volatile uint32_t g_interrupt_state = 0x0;

#if defined( __AVR_ARCH__ )
  static inline
  void enable_interrupts(void)
  {
    if ( g_interrupt_state ) {
      sei();
    }
  }

  static inline
  void disable_interrupts(void)
  {
    g_interrupt_state = (SREG & (1<<SREG_I));
    cli();
  }

#elif defined( SAM )
  static inline
  void enable_interrupts(void)
  {
    if ( g_interrupt_state ) {
      __enable_irq();
    }
  }

  static inline
  void disable_interrupts(void)
  {
    g_interrupt_state = (__get_PRIMASK() == 0);
    __disable_irq();
  }

#elif defined( ESP8266 )
  static inline
  void enable_interrupts(void)
  {
    // Restore the old interrupt state
    xt_wsr_ps(g_interrupt_state);
  }

  static inline
  void disable_interrupts(void)
  {
    /* This turns off interrupts and gets the old state in one function call
     * See https://github.com/esp8266/Arduino/issues/615 for details
     * level 15 will disable ALL interrupts,
     *level 0 will enable ALL interrupts
     */
    g_interrupt_state = xt_rsil(15);
  }

#elif defined( CORE_TEENSY )
  static inline
  void enable_interrupts(void)
  {
    SREG = (uint8_t)g_interrupt_state;     /* restore interrupt state */
  }

  static inline
  void disable_interrupts(void)
  {
    g_interrupt_state = SREG;     /* save interrupt enable/disable state */
    cli();                        /* disable the global interrupt */
  }

#elif defined( HAS_X86 )
  static inline
  void enable_interrupts(void)
  {
  }

  static inline
  void disable_interrupts(void)
  {
  }
#else

#error "Unknown microcontroller:  Need to implement class SuppressInterrupts for this microcontroller."

#endif

INO_API_ENTRY
ino_u8 trigger_event(
  const clock_ts ts,
  const clock_ts last_ts,
  const delay_ts interval_ts);

INO_API_ENTRY
delay_ts elapsed_ms(
  const delay_ts ts,
  const delay_ts last_ts);

INO_API_ENTRY
clock_ts clock_ms(void);

INO_API_ENTRY
clock_ts clock_us(void);

INO_API_ENTRY
void wait_ms(const delay_ts ts);

INO_API_ENTRY
void wait_us(const delay_ts ts);

INO_API_DECLARE_END

INO_NAMESPACE_END

#endif    /*__INO_HAL_ADAPTER_H__*/
