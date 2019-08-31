#ifndef __INO_EVENT_HANDLER_H__
#define __INO_EVENT_HANDLER_H__
#include "InoMyConfig.h"
#include "InoTypes.h"

#include "event_manager_types.h"

#ifndef INO_HANDLER_TIMER_INTERVAL_MS
#define INO_HANDLER_TIMER_INTERVAL_MS           (1000)
#endif

INO_NAMESPACE

typedef bool (*handler_loop_fn)(void* fn_arg);

INO_API_DECLARE

bool handlerPushEvent(
  const uint32_t q, const event_t* event);

bool handlerPushListener(
  const event_code_t event_codes, const event_listener_fn_t listener);

bool handlerPopListener(
  const event_listener_fn_t listener);

bool handlerPushLoop(
  handler_loop_fn fn, void* fn_arg);

bool handlerPopLoop(
  handler_loop_fn fn);

bool handlerInit(
  const event_listener_fn_t default_listener);

bool handlerLoop(
  const uint32_t delay_ms=0);

INO_API_DECLARE_END

INO_NAMESPACE_END

#endif    /*__INO_EVENT_HANDLER_H__*/
