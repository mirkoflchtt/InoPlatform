#include <stdio.h>
#include "InoEventHandler.h"
#include "InoHalAdapter.h"

using namespace ino;

INO_DECLARE_STATIC
ino_u32 listener_cb(
  const ino::EventHandler::Event& event,
  ino_handle cookie)
{
  printf("#### %s called: event(%p) cookie(%p)\n", (const char*)cookie, &event, cookie);
  printf("         event: code(%u) cookie(%p) ts(%u)\n",
    event.get_code(), event.get_cookie(), event.get_timestamp());

  return 0;
}

int main(int argc, char* argv[])
{
  EventHandler handler(16);
  EventHandler::Listener listener1(listener_cb, INO_HANDLE("listener01"));
  EventHandler::Listener listener2(listener_cb, INO_HANDLE("listener02"));

  handler.init(500);
  handler.pushListener(listener1);
  handler.pushListener(listener2);

  EventHandler::event_code code = 0x0;

  while (1) {
    EventHandler::Event event(code, INO_HANDLE(NULL));

    handler.pushEvent(EventHandler::HIGH_PRIORITY, event);

    handler.loop();
    code++;
  }

  return 0;
}

