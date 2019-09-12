#ifndef __INO_STD_BUTTON
#define __INO_STD_BUTTON
#include "InoTypes.h"
#include "InoHalAdapter.h"

INO_NAMESPACE

typedef enum {
  EV_NONE         = 0,    // button is not pressed
  EV_SHORTPRESS   = 1,    // button has been pressed for a short period
  EV_LONGPRESS    = 2,    // button has been pressed for a long period
  EV_TIMEOUT      = 3,    // button has been pressed for a VERY time long period
} StdButtonEvent;

typedef ino_bool (*readStdButtonFunc)(const ino_u8 pin);

class StdButton {
public:
  StdButton(
    const ino_u8 pin,
    const ino_u8 pin_state=LOW, // default mode is pull-down
    const readStdButtonFunc func=NULL,   // if NULL than detect a HIGH on pin
    const ino_u16 longpress_msec=MY_INO_BUTTON_LONG_PRESS,
    const ino_u16 timeout_msec=MY_INO_BUTTON_TIMEOUT,
    const ino_u16 debounce_msec=MY_INO_BUTTON_DEBOUNCE);

  StdButtonEvent  check(void);

protected:
  ino_bool         m_was_pressed;        // previous state
  clock_ts         m_pressed_start;      // start time when 1st pressed

  const ino_u8     m_pin;                // pin to which button is connected
  const ino_u16    m_longpress_msec;     // longpress duration
  const ino_u16    m_timeout_msec;       // timeout duration
  const ino_u16    m_debounce_msec;    // debounce time for the button press
    
  const readStdButtonFunc  m_read_func;
};

INO_NAMESPACE_END

#endif      /*__INO_STD_BUTTON*/

