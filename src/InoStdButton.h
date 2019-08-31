#ifndef __INO_STD_BUTTON__
#define __INO_STD_BUTTON__
#include "InoTypes.h"
#include "InoHalAdapter.h"

INO_NAMESPACE

typedef enum {
  EV_NONE         = 0,    // button is not pressed
  EV_SHORTPRESS   = 1,    // button has been pressed for a short period
  EV_LONGPRESS    = 2,    // button has been pressed for a long period
  EV_TIMEOUT      = 3,    // button has been pressed for a VERY time long period
} StdButtonEvent;

typedef bool (*readStdButtonFunc)(const uint8_t pin);

class StdButton {
public:
  StdButton(
    const uint8_t pin,
    const uint8_t pin_state=LOW, // default mode is pull-down
    const readStdButtonFunc func=NULL,   // if NULL than detect a HIGH on pin
    const uint16_t longpress_msec=MY_INO_BUTTON_LONG_PRESS,
    const uint16_t timeout_msec=MY_INO_BUTTON_TIMEOUT,
    const uint16_t debounce_msec=MY_INO_BUTTON_DEBOUNCE);

  StdButtonEvent  check(void);

protected:
  bool              m_was_pressed;        // previous state
  clock_ts          m_pressed_start;      // start time when 1st pressed

  const uint8_t     m_pin;                // pin to which button is connected
  const uint16_t    m_longpress_msec;     // longpress duration
  const uint16_t    m_timeout_msec;       // timeout duration
  const uint16_t    m_debounce_msec;    // debounce time for the button press
    
  const readStdButtonFunc  m_read_func;
};

INO_NAMESPACE_END

#endif      /*__INO_STD_BUTTON__*/

