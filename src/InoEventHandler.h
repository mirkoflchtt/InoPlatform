#ifndef INO_EVENT_HANDLER_H
#define INO_EVENT_HANDLER_H
#include "InoMyConfig.h"
#include "InoTypes.h"
#include "InoFlags.h"

#include "event_manager.h"

#ifndef INO_HANDLER_TIMER_INTERVAL_MS
#define INO_HANDLER_TIMER_INTERVAL_MS           (1000)
#endif

INO_NAMESPACE

INO_API_DECLARE

ino_bool handlerPushEvent(
  const ino_u32 q,
  const event_t* event);

ino_bool handlerPushListener(
  event_listener_t* listener);

ino_bool handlerPopListener(
  event_listener_t* listener);

ino_bool handlerInit(
  const event_listener_fn_t default_listener,
  listener_handle_t listener_cookie,
  const ino_interval interval_ms=INO_HANDLER_TIMER_INTERVAL_MS);

ino_bool handlerLoop(
  const ino_interval delay_ms=0);

INO_API_DECLARE_END

INO_NAMESPACE_END

#endif    /*INO_EVENT_HANDLER_H*/
