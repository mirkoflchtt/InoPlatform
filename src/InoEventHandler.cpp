//#include <stdio.h>
#include <vector>
#include <Ticker.h>
#include "InoEventHandler.h"
#include "InoHalAdapter.h"

#include "event_manager.h"


INO_NAMESPACE

void timerCB();

class EventHandler {
public:
  EventHandler(void);
  ~EventHandler(void);

  void setState(const uint32_t state)
  {
    m_state |=  state;
  }

  bool pushEvent(
    const uint32_t q, const event_t* event);

  bool pushListener(
    const event_code_t event_codes, const event_listener_fn_t listener);

  bool popListener(
    const event_listener_fn_t listener);

  bool pushLoop(
    handler_loop_fn fn, void* fn_arg);

  bool popLoop(
    handler_loop_fn fn);

  bool init(
    const event_listener_fn_t default_listener);

  bool loop(const uint32_t delay_ms);

private:
  class LoopEntry {
  public:
    LoopEntry(handler_loop_fn fn_, void* fn_arg_) :
      fn(fn_),
      fn_arg(fn_arg_)
    {
    }

    handler_loop_fn       fn;
    void*                 fn_arg;
  };

  event_manager_t               m_manager;
  std::vector<LoopEntry>        m_loop_fn;
  Ticker                        m_timer_low;
  uint32_t                      m_state;
};


EventHandler::EventHandler(void) :
m_manager(),
m_loop_fn(),
m_timer_low(),
m_state(0x0)
{
  event_manager_init(&m_manager, NULL);
}

EventHandler::~EventHandler(void)
{
  event_manager_flush(&m_manager, false);
}

bool EventHandler::pushEvent(
    const uint32_t q, const event_t* event)
{
  return event_manager_push_event(&m_manager, q, event);
}

bool EventHandler::pushListener(
  const event_code_t event_codes, const event_listener_fn_t listener)
{
  return event_manager_bind_listener(&m_manager, event_codes, listener);
}

bool EventHandler::popListener(
  const event_listener_fn_t listener)
{
  return event_manager_unbind_listener(&m_manager, listener);
}

bool EventHandler::pushLoop(
  handler_loop_fn fn, void* fn_arg)
{
  if ( !fn ) return false;

  m_loop_fn.push_back(LoopEntry(fn , fn_arg));
  return true;
}

bool EventHandler::popLoop(
  handler_loop_fn fn)
{
  if ( !fn ) return false;

  for ( std::vector<LoopEntry>::iterator it=m_loop_fn.begin(); 
        it!=m_loop_fn.end(); it++ ) {
    if ( it->fn==fn ) {
      m_loop_fn.erase(it);
      return true;
    }
  }
  return false;
}

bool EventHandler::init(
  const event_listener_fn_t default_listener)
{
  event_manager_init(&m_manager, default_listener);
  m_timer_low.attach(INO_HANDLER_TIMER_INTERVAL_MS*0.001, timerCB);
  return true;
}

bool EventHandler::loop(const uint32_t delay_ms)
{
  const clock_ts _end = clock_ms() + delay_ms;
  bool ok = true;
  
  event_manager_pop_event(&m_manager, 0x0);

  if ( m_state & 0x1 ) {
    m_state &= ~0x1;
    event_manager_pop_event(&m_manager, 0x1);
  }

  for ( std::vector<LoopEntry>::iterator it=m_loop_fn.begin(); 
        it!=m_loop_fn.end(); it++ ) {
    ok &= it->fn(it->fn_arg);
  }

  const uint32_t _now = clock_ms();
  if ( event_manager_queue_is_empty(&m_manager, 0x0) && (_end>_now) ) {
    wait_ms(_end-_now);
  }

  return ok;
}

/******************************************************************************/
static EventHandler g_ino_handler;

void timerCB()
{
  g_ino_handler.setState(0x1);
}

INO_API_DECLARE

bool handlerPushEvent(
  const uint32_t q, const event_t* event)
{
  return g_ino_handler.pushEvent(q, event);
}

bool handlerPushListener(
  const event_code_t event_codes, const event_listener_fn_t listener)
{
  return g_ino_handler.pushListener(event_codes, listener);
}

bool handlerPopListener(
  const event_listener_fn_t listener)
{
  return g_ino_handler.popListener(listener);
}

bool handlerPushLoop(
  handler_loop_fn fn, void* fn_arg)
{
  return g_ino_handler.pushLoop(fn, fn_arg);
}

bool handlerPopLoop(
  handler_loop_fn fn)
{
  return g_ino_handler.popLoop(fn);
}

bool handlerInit(
  const event_listener_fn_t default_listener)
{
  return g_ino_handler.init(default_listener);
}

bool handlerLoop(
  const uint32_t delay_ms)
{
  return g_ino_handler.loop(delay_ms);
}

INO_API_DECLARE_END

INO_NAMESPACE_END
