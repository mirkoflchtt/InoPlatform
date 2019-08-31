#include <InoPlatform.h>

#define BUTTON_PIN        (4)
#define BUTTON_STATE      (LOW)

using namespace ino;

static
void onKeyPressedEvent(AdvButton* b)
{
  Serial.println("onKeyPressedEvent!!");
}


void onKeyDownEvent(AdvButton* b)
{
  Serial.println("onKeyDownEvent!!");
}


void onKeyUpEvent(AdvButton* b)
{
  Serial.println("onKeyUpEvent!!");
}

AdvButton button(
  BUTTON_PIN,
  BUTTON_STATE,
  &onKeyPressedEvent,
  &onKeyDownEvent,
  &onKeyUpEvent);


void setup(void)
{
  Serial.begin(115200);
  Serial.println("InoAdvButtonTest - init done..");
  ino::wait_ms(200);
}

void loop(void)
{
  button.check();
  ino::wait_ms(250);
}
