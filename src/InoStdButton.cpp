#include "InoStdButton.h"

INO_NAMESPACE

static
ino_bool defaultStdReadFunc(const ino_u8 pin)
{
  return (digitalRead(pin)==HIGH);
}

StdButton::StdButton(
  const ino_u8 pin,
  const ino_u8 pin_state,
  const readStdButtonFunc func,
  const ino_u16 longpress_msec,
  const ino_u16 timeout_msec,
  const ino_u16 debounce_msec) :
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
  const ino_bool now_pressed = m_read_func(m_pin);
  StdButtonEvent event   = EV_NONE;
  const ino_timestamp ts = clock_ms();
  
  /* button was released */
  if ( (!now_pressed) && m_was_pressed ) {
    // handle release button event
    //if ( ts>=m_pressed_start+m_debounce_msec ) {
    if ( trigger_event(ts, m_pressed_start, m_debounce_msec) ) {
      //event = (ts<m_pressed_start+m_longpress_msec) ? (EV_SHORTPRESS) : ((ts<m_pressed_start+m_timeout_msec) ? EV_LONGPRESS : EV_TIMEOUT);
      event = trigger_event(ts, m_pressed_start, m_longpress_msec) \
        ? (trigger_event(ts, m_pressed_start, m_timeout_msec) \
          ? EV_TIMEOUT : EV_LONGPRESS) \
        : (EV_SHORTPRESS);
    }
    m_pressed_start = 0;
  }

  /* update press 1st time the button is pressed */
  else if ( now_pressed && (!m_was_pressed)) {
    m_pressed_start = ts;
  }
  
  /* remember state, and we're done */
  m_was_pressed = now_pressed;

  return event;
}

INO_NAMESPACE_END
