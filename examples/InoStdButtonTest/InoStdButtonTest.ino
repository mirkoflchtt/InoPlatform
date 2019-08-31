#include <InoPlatform.h>

#define ANALOG_BUTTON_PIN     (A0)

using namespace ino;

static
bool readButton01(const uint8_t pin)
{
  const int v = analogRead(pin);
  return (v<=40);
}

bool readButton02(const uint8_t pin)
{
  const int v = analogRead(pin);
  return (v>40 && v<=90);
}

bool readButton03(const uint8_t pin)
{
  const int v = analogRead(pin);
  return (v>90 && v<=130);
}

bool readButton04(const uint8_t pin)
{
  const int v = analogRead(pin);
  return (v>130 && v<=170);
}

bool readButton05(const uint8_t pin)
{
  const int v = analogRead(pin);
  return (v>170 && v<=210);
}

bool readButton06(const uint8_t pin)
{
  const int v = analogRead(pin);
  return (v>210 && v<=250);
}

StdButton button01(ANALOG_BUTTON_PIN, HIGH, &readButton01);
StdButton button02(ANALOG_BUTTON_PIN, HIGH, &readButton02);
StdButton button03(ANALOG_BUTTON_PIN, HIGH, &readButton03);
StdButton button04(ANALOG_BUTTON_PIN, HIGH, &readButton04);
StdButton button05(ANALOG_BUTTON_PIN, HIGH, &readButton05);
StdButton button06(ANALOG_BUTTON_PIN, HIGH, &readButton06);


void logButtonEvent(
  const char* event_msg,
  const StdButtonEvent event)
{
  if ( event==EV_NONE )
    return;
    
  Serial.print(event_msg);

    switch (event)
    {
    case EV_SHORTPRESS:
      Serial.println(" EV_SHORTPRESS");
      break;
    case EV_LONGPRESS:
      Serial.println(" EV_LONGPRESS ");
      break;
    default:
      Serial.println(" EV_TIMEOUT   ");
      break;
    }
}

void setup(void)
{
  Serial.begin(115200);
  
  Serial.println("###### InoAnalogButtonTest Init Done ######");
}

void loop(void)
{
  const StdButtonEvent event01 = button01.check();
  const StdButtonEvent event02 = button02.check();
  const StdButtonEvent event03 = button03.check();
  const StdButtonEvent event04 = button04.check();
  const StdButtonEvent event05 = button05.check();
  const StdButtonEvent event06 = button06.check();
  
  logButtonEvent("Button01 :", event01);
  logButtonEvent("Button02 :", event02);
  logButtonEvent("Button03 :", event03);
  logButtonEvent("Button04 :", event04);
  logButtonEvent("Button05 :", event05);
  logButtonEvent("Button06 :", event06);
  
  ino::wait_ms(200);
}
