#include "InoStdButton.h"

INO_NAMESPACE

static
bool defaultStdReadFunc(const uint8_t pin)
{
  return (digitalRead(pin)==HIGH);
}

StdButton::StdButton(
  const uint8_t pin,
  const uint8_t pin_state,
  const readStdButtonFunc func,
  const uint16_t longpress_msec,
  const uint16_t timeout_msec,
  const uint16_t debounce_msec) :
m_was_pressed(false),
m_pressed_start(0),
m_pin(pin),
m_longpress_msec(longpress_msec),
m_timeout_msec(timeout_msec),
m_debounce_msec(debounce_msec),
m_read_func((func) ? func : &defaultStdReadFunc)
{
  pinMode(m_pin, INPUT);
  digitalWrite(m_pin, pin_state); // if HIGH than enable pull-up
}


StdButtonEvent StdButton::check(void)
{
  const bool now_pressed  = m_read_func(m_pin);
  StdButtonEvent event    = EV_NONE;
  const clock_ts  _now    = clock_ms();
  
  // button was released
  if ( !now_pressed && m_was_pressed ) {
    // handle release button event
    if ( _now>=m_pressed_start+m_debounce_msec ) {
      event = (_now<m_pressed_start+m_longpress_msec) ? (EV_SHORTPRESS) : ((_now<m_pressed_start+m_timeout_msec) ? EV_LONGPRESS : EV_TIMEOUT);
    }
    m_pressed_start = 0;
  }

  // update press 1st time the button is pressed
  else if ( !m_was_pressed && now_pressed ) {
    m_pressed_start = _now;
  }
  
  // remember state, and we're done
  m_was_pressed = now_pressed;

  return event;
}

INO_NAMESPACE_END
