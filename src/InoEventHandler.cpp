//#include <stdio.h>

#include "InoAssert.h"
#include "InoHalAdapter.h"
#include "InoEventHandler.h"

#include "ino_dlist.h"
#include "ino_rbuffer.h"

#define INO_HANDLER_QUEUE_NUM                   (2)
#define INO_HANDLER_FLAG_TIMER_TRIGGER          (0x1)

/****************************************************************************/
#define INO_HANDLER_QUEUE(queue_id_) \
  (((ino_rbuffer*)m_event_queue)+(queue_id_))

#define TO_DLIST(list_) \
  ((ino_dlist*)&(list_))

INO_NAMESPACE

/****************************************************************************/
EventHandler::Event::Event(void) :
m_code(0x0),
m_payload_u32(0),
m_timestamp(clock_ms())
{
}

EventHandler::Event::Event(const Event::event_code code) :
m_code(code),
m_payload_u32(0),
m_timestamp(clock_ms())
{
}

EventHandler::Event::Event(const Event::event_code code, const ino_i32 value) :
m_code(code),
m_payload_i32(value),
m_timestamp(clock_ms())
{
}

EventHandler::Event::Event(const Event::event_code code, const ino_u32 value) :
m_code(code),
m_payload_u32(value),
m_timestamp(clock_ms())
{
}

EventHandler::Event::Event(const Event::event_code code, const ino_float value) :
m_code(code),
m_payload_f32(value),
m_timestamp(clock_ms())
{
}

EventHandler::Event::Event(const Event::event_code code, const ino_handle payload) :
m_code(code),
m_payload(payload),
m_timestamp(clock_ms())
{
}

EventHandler::Event::event_code EventHandler::Event::get_code(void) const
{
  return m_code;
}

ino_timestamp EventHandler::Event::get_timestamp(void) const
{
  return m_timestamp;
}

ino_handle EventHandler::Event::get_payload(void) const
{
  return m_payload;
}

ino_i32 EventHandler::Event::get_i32(void) const
{
  return m_payload_i32;
}

ino_u32 EventHandler::Event::get_u32(void) const
{
  return m_payload_u32;
}

ino_float EventHandler::Event::get_float(void) const
{
  return m_payload_f32;
}

ino_bool EventHandler::Event::trigger(const Event::event_code code) const
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
  const Event::event_code event_codes,
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
}

EventHandler::~EventHandler(void)
{
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

ino_bool EventHandler::pushEventTemperature(
  const ino_u8 sensor_id, const ino_float temperature)
{
  const ino_i32 payload = ((INO_ROUND(temperature * 100.0f) << 8) | sensor_id);
  // printf("  pushEventTemperature: id(%u) temp(%f) payload(%d)\n", sensor_id, temperature, payload);
  return pushEvent(LOW_PRIORITY, Event(Events::GOT_TEMPERATURE, payload));
}

ino_bool EventHandler::pushEventHumidity(
  const ino_u8 sensor_id, const ino_float humidity)
{
  const ino_i32 payload = ((INO_ROUND(humidity * 100.0f) << 8) | sensor_id);
  // printf("  pushEventHumidity: id(%u) hum(%f) payload(%d)\n", sensor_id, humidity, payload);
  return pushEvent(LOW_PRIORITY, Event(Events::GOT_HUMIDITY, payload));
}

ino_bool EventHandler::parseEventTemperature(
  const Event& event, ino_u8& sensor_id, ino_float& temperature)
{
  if (event.get_code()==Events::GOT_TEMPERATURE)
  {
    const ino_i32 payload = event.get_i32();
    sensor_id = (payload & 0xFF);
    temperature = (payload >> 8) * 0.01f;
    // printf("  parseEventTemperature: id(%u) temp(%f) payload(%d)\n", sensor_id, temperature, payload);
    return true;
  }
  return false;
}

ino_bool EventHandler::parseEventHumidity(
  const Event& event, ino_u8& sensor_id, ino_float& humidity)
{
  if (event.get_code()==Events::GOT_HUMIDITY)
  {
    const ino_i32 payload = event.get_i32();
    sensor_id = (payload & 0xFF);
    humidity = (payload >> 8) * 0.01f;
    // printf("  parseEventHumidity: id(%u) hum(%f) payload(%d)\n", sensor_id, humidity, payload);
    return true;
  }
  return false;
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
  ino_bool ok;
  disable_interrupts();
  ok = ino_rbuffer_pop(buffer, &event);
  enable_interrupts();

  return (ok) ? sendEvent(event) : 0;
}

ino_size EventHandler::sendEvent(
  const Event& event)
{
  ino_size count = 0;

  INO_DLIST_FOR_EACH_OBJ_NEXT(l, &m_listeners, Listener, m_list)
  {
    count += (l->trigger(event)) ? 1 : 0;
  }

  return count;
}

ino_bool EventHandler::pushQueueEvent(
    const queue_type q,
    const Event& event)
{
  ino_rbuffer* buffer = INO_HANDLER_QUEUE(q);
  ino_bool ok;

  disable_interrupts();
  ok = ino_rbuffer_is_full(buffer);
  enable_interrupts();
  if (ok) {
    popEvent(q);
  }
  disable_interrupts();
  ok = ino_rbuffer_push(buffer, INO_HANDLE(&event));
  enable_interrupts();
  return ok;
}

ino_size EventHandler::processQueueEvent(
  const queue_type q)
{
  return popEvent(q);
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

#undef INO_HANDLER_QUEUE
#undef INO_HANDLER_QUEUE_NUM
#undef TO_DLIST

INO_API_DECLARE

INO_API_DECLARE_END

INO_NAMESPACE_END
