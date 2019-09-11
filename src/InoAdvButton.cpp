/*
*
* Thanks for using this library! If you like it, please drop me a comment at bart@sbo-dewindroos.nl.
*
* File     : AdvButton.cpp
* Version  : 1.0
* Released : 24/01/2010
* Author   : Bart Meijer (bart@sbo-dewindroos.nl)
*
* This is the Advanced Button library for the Arduino platform. It will enable you to easily 
* react to user input using simple press-to-make buttons. Features include:
* - Event based implementation 
* - Recording the time a button is pressed
* - Adjustable repeat delay, start delay for the keypressed event
* - requires only a single call in the main loop
*
* This file implements the AdvButton class
*
*/
#include "InoAdvButton.h"
//#include "ButtonManager.h"

INO_NAMESPACE

uint16_t defaultReadPin(const uint8_t pin)
{
  return digitalRead(pin);
}

AdvButton::AdvButton(
  const uint8_t pin,
  const uint8_t offState,
  void (*OnKeyPress)(AdvButton*),
  void (*OnKeyDown)(AdvButton*),
  void (*OnKeyUp)(AdvButton*),
  functionReadPin funcReadPin,
  const uint16_t repeat,
  const uint16_t startDelay) :
m_funcReadPin(NULL),
m_lastChange(clock_ms()),
m_offState(offState),
m_lastState(offState),
m_debounceTime(100)
{
  setPin(pin, offState, funcReadPin);
  setRepeat(repeat);
  setStartDelay(startDelay);
  setOnKeyPress((*OnKeyPress));
  setOnKeyDown((*OnKeyDown));
  setOnKeyUp((*OnKeyUp));
  
//  ButtonManager::instance()->addButton(this);
}

void AdvButton::check(void)
{
  // determine current pin state
  const uint16_t cur = m_funcReadPin(m_pin);

  /* button state changed */
  if ( m_lastState!=cur )
  {
    const clock_ts ts = clock_ms();
    //if ( ts<m_lastChange+m_debounceTime ) {
    if ( !trigger_event(ts, m_lastChange, m_debounceTime) ) {
      return;
    }
      
    m_lastChange = ts;
    m_lastState  = cur; 
  }

  if ( cur==m_offState )
  {
    /* the button is released, but was it previously pressed */   
    if ( m_startPress && m_funcKeyUp ) {
      m_funcKeyUp(this);
    }
      
    m_startPress = 0;
    m_prevPres   = 0;
  }
  else
  { 
    // the button is pressed, but last time we checked, was the button still up?    
    if ( m_startPress==0 ) {
      /* mark the start time and notify others */
      m_startPress = clock_ms();
      if ( m_funcKeyDown ) {
        m_funcKeyDown(this);
      }
      if ( m_funcKeyPress ) {
        m_funcKeyPress(this); 
      }
    }
    
    /* is repeating enabled? */
    if ( (m_repeat>0) && (m_funcKeyPress) )
    {
      const clock_ts ts = clock_ms();
      /* is the startdelay passed? */
      //if ( ts>=m_startPress+m_startDelay ) {
      if ( trigger_event(ts, m_startPress, m_startDelay) ) {
        /* is it time for a keypressed call? */
        //if ( ts>m_prevPres+m_repeat ) {
        if ( trigger_event(ts, m_prevPres, m_repeat) ) {
          m_prevPres = ts;   
          m_funcKeyPress(this);   
        }
      }
      else {
        m_prevPres = ts;
      }
    }
  }
}

delay_ts AdvButton::getPressTime(void)
{
  return ino::elapsed_ms(ino::clock_ms(), m_startPress);
}

void AdvButton::setOnKeyPress(void (*f)(AdvButton*))
{
  m_funcKeyPress =  (*f);
}

void AdvButton::setOnKeyDown(void (*f)(AdvButton*))
{
  m_funcKeyDown =  (*f);
}

void AdvButton::setOnKeyUp(void (*f)(AdvButton*))
{
  m_funcKeyUp =  (*f);
}

void AdvButton::setRepeat(const uint16_t repeat)
{
  m_repeat = repeat;
}

void AdvButton::setStartDelay(const uint16_t startDelay)
{
  m_startDelay = startDelay;
}

void AdvButton::setPin(
  const uint8_t pin,
  const uint8_t offState,
  functionReadPin funcReadPin)
{
  m_pin         = pin;
  m_offState    = offState;
  m_lastState   = offState;
  m_funcReadPin = (funcReadPin) ? funcReadPin : &defaultReadPin;
  
  pinMode(m_pin, OUTPUT);
  digitalWrite(m_pin, m_offState);
}

INO_NAMESPACE_END
