//#include <stdio.h>

#include "InoAssert.h"
#include "InoHalAdapter.h"
#include "InoEventHandler.h"

#include "ino_dlist.h"
#include "ino_rbuffer.h"

#define INO_HANDLER_FLAG_TIMER_TRIGGER          (0x1)

#define INO_HANDLER_PAYLOAD_SET(value)          *(((ino_handle*)&(value)))
#define INO_HANDLER_PAYLOAD_GET(payload, type)  *(((type*)&(payload)))
#define INO_HANDLER_QUEUE(queue_id)             (((ino_rbuffer*)m_event_queue)+(queue_id))        
#define INO_HANDLER_QUEUE_NUM                   (2)

#define TO_DLIST(list)                          ((ino_dlist*)&(list))

INO_NAMESPACE

/****************************************************************************/
EventHandler::Event::Event(void) :
m_code(0x0),
m_payload(NULL),
m_timestamp(clock_ms())
{
}

EventHandler::Event::Event(const event_code code, const ino_i32 value) :
m_code(code),
m_payload(INO_HANDLER_PAYLOAD_SET(value)),
m_timestamp(clock_ms())
{
}

EventHandler::Event::Event(const event_code code, const ino_u32 value) :
m_code(code),
m_payload(INO_HANDLER_PAYLOAD_SET(value)),
m_timestamp(clock_ms())
{
}

EventHandler::Event::Event(const event_code code, const ino_float value) :
m_code(code),
m_payload(INO_HANDLER_PAYLOAD_SET(value)),
m_timestamp(clock_ms())
{
}

EventHandler::Event::Event(const event_code code, const ino_handle cookie) :
m_code(code),
m_payload(INO_HANDLER_PAYLOAD_SET(cookie)),
m_timestamp(clock_ms())
{
}

EventHandler::event_code EventHandler::Event::get_code(void) const
{
  return m_code;
}

ino_timestamp EventHandler::Event::get_timestamp(void) const
{
  return m_timestamp;
}

ino_handle EventHandler::Event::get_cookie(void) const
{
  return INO_HANDLER_PAYLOAD_GET(m_payload, ino_handle);
}

ino_i32 EventHandler::Event::get_i32(void) const
{
  return INO_HANDLER_PAYLOAD_GET(m_payload, ino_i32);
}

ino_u32 EventHandler::Event::get_u32(void) const
{
  return INO_HANDLER_PAYLOAD_GET(m_payload, ino_u32);
}

ino_float EventHandler::Event::get_float(void) const
{
  return INO_HANDLER_PAYLOAD_GET(m_payload, ino_float);
}

ino_bool EventHandler::Event::trigger(const event_code code) const
{
  return ((m_code & code) == m_code);
}

/****************************************************************************/
EventHandler::Listener::Listener(
  listener_cb callback,
  ino_handle cookie) :
m_event_codes(FLAG_ALL),
m_flags(FLAG_NONE),
m_callback(callback),
m_cookie(cookie),
m_list()
{
  INO_DLIST_INIT(&m_list)
}

EventHandler::Listener::Listener(
  const event_code event_codes,
  listener_cb callback,
  ino_handle cookie) :
m_event_codes(event_codes),
m_flags(FLAG_NONE),
m_callback(callback),
m_cookie(cookie),
m_list()
{
  INO_DLIST_INIT(&m_list)
}

ino_bool EventHandler::Listener::trigger(
  const Event& event)
{
  const ino_bool trigger = (valid()) ? event.trigger(m_event_codes) : false;
  if (trigger) {
    //printf("### trigger(%d) event->code(0x%08x) l->codes(0x%08x) AND(0x%08x)\n", trigger, event->code, l->event_codes, (event->code & l->event_codes));

    INO_ASSERT(m_callback)
    m_callback(event, m_cookie);
  }
  return trigger;
}

ino_bool EventHandler::Listener::valid(void)
{
  return (m_callback && m_event_codes && enable()) ? true : false;
}

ino_bool EventHandler::Listener::enable(void) const
{
  return (INO_FLAG_GET(m_flags, FLAG_DISABLE)) ? false : true;
}

void EventHandler::Listener::enable(const ino_bool value)
{
  if (value) {
    INO_FLAG_UNSET(m_flags, FLAG_DISABLE)
  } else {
    INO_FLAG_SET(m_flags, FLAG_DISABLE)
  }
}

/****************************************************************************/
class EventHandler;

INO_DECLARE_STATIC
void GlobalEventHandlerCallback(EventHandler* handler)
{
  INO_ASSERT(handler)
  handler->trigger();
}

EventHandler::EventHandler(
  const ino_size event_queue_size) :
m_listeners(),
m_event_queue(NULL),
m_event_queue_num(0),
m_timer_low()
{
  INO_FLAG_CLEAR(m_state)
  INO_DLIST_INIT(&m_listeners)
  
  allocateEventQueues(event_queue_size, INO_HANDLER_QUEUE_NUM);
  //event_manager_init(&m_manager, NULL, NULL);
}

EventHandler::~EventHandler(void)
{
  //event_manager_flush(&m_manager, false);

  flushQueue(HIGH_PRIORITY);
  flushQueue(LOW_PRIORITY);

  m_timer_low.detach();
  INO_FREE(m_event_queue)
  m_event_queue_num = 0;
}

/****************************************************************************/
/* Public Methods */

void EventHandler::trigger(void)
{
  INO_FLAG_SET(m_state, INO_HANDLER_FLAG_TIMER_TRIGGER);
}

ino_bool EventHandler::pushEvent(
    const queue_type q,
    const Event& event)
{
  return pushQueueEvent(q, event);
}

ino_size EventHandler::getEventsNum(
  const queue_type q) const
{
  disable_interrupts();
  const ino_size num = ino_rbuffer_get_size(INO_HANDLER_QUEUE(q));
  enable_interrupts();
  return num;
}

ino_bool EventHandler::pushListener(
  Listener& listener)
{
  if (!listener.valid()) return false;

  disable_interrupts();
  INO_DLIST_INIT(&listener.m_list)
  INO_DLIST_PUSH_FRONT(TO_DLIST(listener.m_list), TO_DLIST(m_listeners))
  enable_interrupts();

  return true;
}

ino_bool EventHandler::popListener(
  Listener& listener)
{
  disable_interrupts();
  INO_DLIST_OBJ_REMOVE(&listener, m_list)
  INO_DLIST_INIT(&listener.m_list)
  enable_interrupts();

  return true;
}

ino_size EventHandler::getListenersNum(void) const
{
  disable_interrupts();
  const ino_size num = INO_DLIST_SIZE(TO_DLIST(m_listeners));
  enable_interrupts();
  return num;
}

ino_bool EventHandler::init(
  const ino_interval interval_ms)
{
  disable_interrupts();

  INO_DLIST_INIT(&m_listeners)

  for (ino_size q=0; m_event_queue && q<m_event_queue_num; q++)
  {
    ino_rbuffer* buffer = INO_HANDLER_QUEUE(q);
    ino_rbuffer_clear(buffer);
  }
  enable_interrupts();

  if (interval_ms>0) {
    m_timer_low.attach_ms(interval_ms, GlobalEventHandlerCallback, this);
  } else {
    m_timer_low.detach();
  }

  return true;
}

ino_bool EventHandler::loop(void)
{
  ino_size count = 0;
  count += processQueueEvent(HIGH_PRIORITY);

  if (INO_FLAG_GET(m_state, INO_HANDLER_FLAG_TIMER_TRIGGER)) {
    INO_FLAG_UNSET(m_state, INO_HANDLER_FLAG_TIMER_TRIGGER)
    count += processQueueEvent(LOW_PRIORITY);
  }

  return (count > 0) ? true : false;
}

/****************************************************************************/
/* Private Methods */

void EventHandler::allocateEventQueues(
  const ino_size event_queue_size,
  const ino_size event_queue_num)
{
  const ino_size queue_byte_size = event_queue_size * sizeof(Event); 
  const ino_size size = event_queue_num * (sizeof(ino_rbuffer) + queue_byte_size);
  m_event_queue = INO_MALLOC(size, ino_u8);
  m_event_queue_num = (m_event_queue) ? event_queue_num : 0;
  for (ino_size q=0; m_event_queue && q<m_event_queue_num; q++)
  {
    ino_rbuffer* buffer = INO_HANDLER_QUEUE(q);
    ino_ptr data = INO_PTR(INO_HANDLER_QUEUE(m_event_queue_num));
    ino_rbuffer_init(buffer, event_queue_size, sizeof(Event), data + q * queue_byte_size);
    ino_rbuffer_clear(buffer);
  }
}

ino_size EventHandler::popEvent(
  const queue_type q)
{
  ino_rbuffer* buffer = INO_HANDLER_QUEUE(q);
  Event event;
  if (ino_rbuffer_pop(buffer, &event)) {
    return sendEvent(event);
  }
  return 0;
}

ino_size EventHandler::sendEvent(
  const Event& event)
{
  ino_size count = 0;
  ino_bool triggered = false;

  INO_DLIST_FOR_EACH_OBJ_NEXT(l, &m_listeners, Listener, m_list)
  {
    triggered = l->trigger(event);
    count += (triggered) ? 1 : 0;
  }

  return count;
}

ino_bool EventHandler::pushQueueEvent(
    const queue_type q,
    const Event& event)
{
  ino_rbuffer* buffer = INO_HANDLER_QUEUE(q);
  disable_interrupts();
  if (ino_rbuffer_is_full(buffer)) {
    popEvent(q);
  }
  const ino_bool ok = ino_rbuffer_push(buffer, INO_HANDLE(&event));
  enable_interrupts();
  return ok;
}

ino_size EventHandler::processQueueEvent(
  const queue_type q)
{
  ino_size count = 0;

  disable_interrupts();
  count = popEvent(q);
  enable_interrupts();

  if (q!=HIGH_PRIORITY) {
    // printf("### [DBG] ts(%u) processQueueEvent(%d) count(%u) qsize(%u)" INO_CR, ino::clock_ms(), q, count, qsize);
  }

  return count;
}

ino_size EventHandler::flushQueue(
  const queue_type q)
{
  ino_size flushed = 0;
  while (processQueueEvent(q) > 0) {
    flushed++;
  }
  return flushed;
}

#undef TO_DLIST

INO_API_DECLARE

INO_API_DECLARE_END

INO_NAMESPACE_END
