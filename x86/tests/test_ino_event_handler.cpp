#include <stdio.h>
#include "InoEventHandler.h"
#include "InoHalAdapter.h"

using namespace ino;

INO_DECLARE_STATIC
ino_bool loop_fn(
  ino_handle arg)
{
  printf("## handler_fn(%p) called\n", arg);
  return true;
}

INO_DECLARE_STATIC
void listener_fn(
  const event_t* event,
  listener_handle_t cookie)
{
  printf("## listener_fn called: event(%p) cookie(%p)\n", event, cookie);
  printf("     event: code(%u) cookie(%p) ts(%u)\n",
    event->code, event->cookie, event->timestamp);
}

INO_DECLARE_STATIC
void listener2_fn(
  const event_t* event,
  listener_handle_t cookie)
{
  printf("#### listener2_fn called: event(%p) cookie(%p)\n", event, cookie);
  printf("         event: code(%u) cookie(%p) ts(%u)\n",
    event->code, event->cookie, event->timestamp);
}

int main(int argc, char* argv[])
{
  event_t event;
  event_listener_t listener[2];

  if (!handlerInit(listener_fn, INO_HANDLE_NULL)) {
    printf("  ## handlerInit failed: exit\n");
    return -1;
  }

  event.code = 0x0;
  listener[0].event_codes = 0x1;
  listener[0].callback = listener2_fn;
  listener[0].cookie = INO_HANDLE(&listener2_fn);
  listener[1] = listener[0];
  listener[1].event_codes = 22;

  if (!handlerPushListener(&listener[0]))
  {
    printf("  ## handlerPushListener failed: exit\n");
    return -2;
  }
  if (!handlerPushListener(&listener[1]))
  {
    printf("  ## handlerPushListener failed: exit\n");
    return -3;
  }

  while (1)
  {
    event.timestamp = clock_ms();
    event.cookie = INO_HANDLE(&listener_fn);

    printf("millis(%u)\n", event.timestamp);

    handlerPushEvent(HIGH_PRIORITY_QUEUE, &event);
    event.code++;

    handlerLoop(1000);
  }
  return 0;
}
