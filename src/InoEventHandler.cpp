//#include <stdio.h>
#include <vector>
#include <Ticker.h>
#include "InoEventHandler.h"
#include "InoHalAdapter.h"

#define STATE_TIMER_TRIGGER       (0x1U)

INO_NAMESPACE

void timerCB(void);

class EventHandler {
public:
  EventHandler(void);
  ~EventHandler(void);

  void setState(const ino_u32 state)
  {
    m_state |=  state;
  }

  ino_bool pushEvent(
    const ino_u32 q,
    const event_t* event);

  ino_bool pushListener(
    event_listener_t* listener);

  ino_bool popListener(
    event_listener_t* listener);

  ino_bool pushLoop(
    handler_loop_fn fn,
    ino_handle fn_arg);

  ino_bool popLoop(
    handler_loop_fn fn);

  ino_bool init(
    const event_listener_fn_t default_listener,
    listener_handle_t listener_cookie,
    const ino_u32 interval_ms);

  ino_bool loop(
    const ino_u32 delay_ms);

private:
  class LoopEntry {
  public:
    LoopEntry(handler_loop_fn fn, ino_handle fn_arg) :
      m_fn(fn),
      m_fn_arg(fn_arg)
    {
    }

    handler_loop_fn             m_fn;
    ino_handle                  m_fn_arg;
  };

  event_manager_t               m_manager;
  std::vector<LoopEntry>        m_loop_fn;
  Ticker                        m_timer_low;
  ino_u32                       m_state;
};


EventHandler::EventHandler(void) :
m_manager(),
m_loop_fn(),
m_timer_low(),
m_state(0x0)
{
  event_manager_init(&m_manager, NULL, NULL);
}

EventHandler::~EventHandler(void)
{
  event_manager_flush(&m_manager, false);
  m_timer_low.detach();
}

ino_bool EventHandler::pushEvent(
    const ino_u32 q,
    const event_t* event)
{
  return event_manager_push_event(&m_manager, q, event);
}

ino_bool EventHandler::pushListener(
  event_listener_t* listener)
{
  return event_manager_bind_listener(
    &m_manager, listener);
}

ino_bool EventHandler::popListener(
  event_listener_t* listener)
{
  return event_manager_unbind_listener(&m_manager, listener);
}

ino_bool EventHandler::pushLoop(
  handler_loop_fn fn,
  ino_handle fn_arg)
{
  if ( !fn ) return false;

  m_loop_fn.push_back(LoopEntry(fn , fn_arg));
  return true;
}

ino_bool EventHandler::popLoop(
  handler_loop_fn fn)
{
  if ( !fn ) return false;

  for ( std::vector<LoopEntry>::iterator it=m_loop_fn.begin(); 
        it!=m_loop_fn.end(); it++ ) {
    if ( it->m_fn==fn ) {
      m_loop_fn.erase(it);
      return true;
    }
  }
  return false;
}

ino_bool EventHandler::init(
  const event_listener_fn_t default_listener,
  listener_handle_t listener_cookie,
  const ino_u32 interval_ms)
{
  event_manager_init(&m_manager, default_listener, listener_cookie);
  m_timer_low.attach_ms(interval_ms, timerCB);
  return true;
}

ino_bool EventHandler::loop(
  const ino_u32 delay_ms)
{
  const ino_timestamp _start = clock_ms();
  ino_bool ok = true;
  
  event_manager_pop_event(&m_manager, HIGH_PRIORITY_QUEUE);

  if (m_state & STATE_TIMER_TRIGGER) {
    m_state &= (~STATE_TIMER_TRIGGER);
    event_manager_pop_event(&m_manager, LOW_PRIORITY_QUEUE);
  }

  for (std::vector<LoopEntry>::iterator it=m_loop_fn.begin(); 
       it!=m_loop_fn.end(); it++ )
  {
       ok &= it->m_fn(it->m_fn_arg);
  }

  const ino_interval _elapsed = elapsed_ms(clock_ms(), _start);
  if (event_manager_queue_is_empty(&m_manager, HIGH_PRIORITY_QUEUE) && (_elapsed<delay_ms) ) {
    wait_ms(delay_ms-_elapsed);
  }

  return ok;
}

/******************************************************************************/
static EventHandler g_ino_handler;

void timerCB(void)
{
  g_ino_handler.setState(STATE_TIMER_TRIGGER);
}

INO_API_DECLARE

ino_bool handlerPushEvent(
  const ino_u32 q,
  const event_t* event)
{
  return g_ino_handler.pushEvent(q, event);
}

ino_bool handlerPushListener(
  event_listener_t* listener)
{
  return g_ino_handler.pushListener(listener);
}

ino_bool handlerPopListener(
  event_listener_t* listener)
{
  return g_ino_handler.popListener(listener);
}

ino_bool handlerPushLoop(
  handler_loop_fn fn,
  ino_handle fn_arg)
{
  return g_ino_handler.pushLoop(fn, fn_arg);
}

ino_bool handlerPopLoop(
  handler_loop_fn fn)
{
  return g_ino_handler.popLoop(fn);
}

ino_bool handlerInit(
  const event_listener_fn_t default_listener,
  listener_handle_t listener_cookie,
  const ino_u32 interval_ms)
{
  return g_ino_handler.init(default_listener, listener_cookie, interval_ms);
}

ino_bool handlerLoop(
  const ino_u32 delay_ms)
{
  return g_ino_handler.loop(delay_ms);
}

INO_API_DECLARE_END

INO_NAMESPACE_END
