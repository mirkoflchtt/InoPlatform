#ifndef __INO_EVENT_HANDLER_H__
#define __INO_EVENT_HANDLER_H__
#include "InoMyConfig.h"
#include "InoTypes.h"

#include "event_manager.h"

#ifndef INO_HANDLER_TIMER_INTERVAL_MS
#define INO_HANDLER_TIMER_INTERVAL_MS           (1000)
#endif

INO_NAMESPACE

typedef ino_bool (*handler_loop_fn)(ino_handle fn_arg);

INO_API_DECLARE

ino_bool handlerPushEvent(
  const ino_u32 q,
  const event_t* event);

ino_bool handlerPushListener(
  event_listener_t* listener);

ino_bool handlerPopListener(
  event_listener_t* listener);

ino_bool handlerPushLoop(
  handler_loop_fn fn, ino_handle fn_arg);

ino_bool handlerPopLoop(
  handler_loop_fn fn);

ino_bool handlerInit(
  const event_listener_fn_t default_listener,
  listener_handle_t listener_cookie,
  const ino_u32 interval_ms=INO_HANDLER_TIMER_INTERVAL_MS);

ino_bool handlerLoop(
  const ino_u32 delay_ms=0);

INO_API_DECLARE_END

INO_NAMESPACE_END

#endif    /*__INO_EVENT_HANDLER_H__*/
