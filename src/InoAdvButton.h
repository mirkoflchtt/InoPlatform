/*
*
* Thanks for using this library! If you like it, please drop me a comment at bart@sbo-dewindroos.nl.
*
* File     : AdvButton.h
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
* This file defines the AdvButton class
*
* Usage: 
* - Include AdvButton.h and ButtonManager.h in your sketch
* - Add a call to ButtonManager::instance()->checkButtons(); in your main loop
* - Declare each button and define the events using a overload of AdvButton ( AdvButton button = AdvButton(<pin>) )
* - Declare the required event functions ( void OnKeyXXX(AdvButton* but) )
* - See the comments below for more help
*/
#ifndef __INO_ADVBUTTON_H__
#define __INO_ADVBUTTON_H__
#include "InoTypes.h"
#include "InoHalAdapter.h"

INO_NAMESPACE

class AdvButton;

typedef void  (*functionType)( AdvButton* );
typedef uint16_t (*functionReadPin)( const uint8_t pin );

class AdvButton
{

public:
  /*
  This is the first constructor call, use it for keypress buttons 
  pin        : pin number of the button
        OnKeyPress : Function to call when the event occurs
      repeat     : time between the event is raised while pressing the button (optional)
  startDelay : amount of time between the initial keypress event and when to start repeating (optional)
  */
  AdvButton(
    const uint8_t pin,
    const uint8_t offState,
    void (*OnKeyPress)(AdvButton*),
    void (*OnKeyDown)(AdvButton*) = NULL,
    void (*OnKeyUp)(AdvButton*)   = NULL,
    functionReadPin funcReadPin   = NULL,
    const uint16_t repeat     = 300,
    const uint16_t startDelay   = 500 );

  /* 
  Checks the state of the button and triggers events accordingly
  Will be called from the ButtonManager 
  */
  void check( void );

  /* 
  This function will set the function to call when the keypress event occurs
  */
  void setOnKeyPress(void (*f)(AdvButton*));

  /* 
  This function will set the function to call when the keydown event occurs
  */
  void setOnKeyDown(void (*f)(AdvButton*));

  /* 
  This function will set the function to call when the keyup event occurs
  */
  void setOnKeyUp(void (*f)(AdvButton*));

  /* 
  Sets the time (milliseconds) between each repeated keypress event
  */
  void setRepeat( const uint16_t repeat );

  /* 
  Sets the delay (milliseconds) before the keypress event is repeated
  */
  void setStartDelay( const uint16_t startDelay );

  /* 
  Changes the pin number 
  */
  void setPin(
    const uint8_t pin,
    const uint8_t offState,
    functionReadPin funcReadPin=NULL );
  
  /* 
  Retrieve the amount of milliseconds the button was pressed,  only valid in keyevents
  */
  ino::delay_ts getPressTime( void );
  
private: 
  /* event callbacks */
  functionType  m_funcKeyUp;
  functionType  m_funcKeyDown;
  functionType  m_funcKeyPress;
  functionReadPin m_funcReadPin;
  
  ino::clock_ts m_startPress;
  ino::clock_ts m_lastChange;
  ino::clock_ts m_prevPres;
  
  uint8_t     m_pin;
  uint16_t    m_offState;
  uint16_t    m_lastState;
  uint16_t    m_debounceTime;
  uint16_t    m_repeat;
  uint16_t    m_startDelay;
};

INO_NAMESPACE_END

#endif    /*__INO_ADVBUTTON_H__*/
