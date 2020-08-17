#ifndef INO_EVENT_HANDLER_H
#define INO_EVENT_HANDLER_H
#include <Ticker.h>

#include "InoMyConfig.h"
#include "InoTypes.h"
#include "InoFlags.h"


#ifndef INO_HANDLER_TIMER_INTERVAL_MS
#define INO_HANDLER_TIMER_INTERVAL_MS       (1000)
#endif

INO_NAMESPACE

class EventHandler {
public:
  typedef ino_i32 event_code;

  typedef enum {
    HIGH_PRIORITY       = 0x0,
    LOW_PRIORITY        = 0x1,
  } queue_type;

  class Event {
  public:
    Event(void);
    Event(const event_code code, const ino_i32 value);
    Event(const event_code code, const ino_u32 value);
    Event(const event_code code, const ino_float value);
    Event(const event_code code, const ino_handle cookie);

    event_code get_code(void) const;
    ino_timestamp get_timestamp(void) const;
    ino_handle get_cookie(void) const;
    ino_i32 get_i32(void) const;
    ino_u32 get_u32(void) const;
    ino_float get_float(void) const;

    ino_bool trigger(const event_code code) const;

  private:
    const event_code      m_code;

    /*! Opaque cookie handle passed by the event to the listener. */
    const ino_handle      m_payload;

    /*! Timestamp when the event was generated. */
    ino_timestamp         m_timestamp;
  };

  class Listener {
  public:
    typedef ino_u32 (*listener_cb)(
      const Event& event, ino_handle cookie);

    Listener(
      listener_cb callback=NULL,
      ino_handle cookie=NULL);

    Listener(
      const event_code event_codes,
      listener_cb callback,
      ino_handle cookie=NULL);

    ino_bool trigger(const Event& event);
    ino_bool valid(void);

  private:
    enum {
      FLAG_NONE = 0x0U,
      FLAG_ALL = ~0x0U,
      FLAG_DISABLE = 0x1U,
    };

    event_code      m_event_codes;
    ino_flags       m_flags;

    /*! Listener function. */
    listener_cb     m_callback;
    ino_handle      m_cookie;

  public:
    typedef struct s_listener_list {
      struct s_listener_list*     next; /*!< pointer to the next element of the list */
      struct s_listener_list*     prev; /*!< pointer to the previous element of the list */
    } listener_list;

    ino_bool enable(void) const;
    void enable(const ino_bool value);

    listener_list   m_list;
  };

  EventHandler(
    const ino_size event_queue_size);
  
  ~EventHandler(void);

  void trigger(void);

  ino_bool pushEvent(
    const queue_type q,
    const Event& event);

  ino_size getEventsNum(
    const queue_type q) const;

  ino_bool pushListener(
    Listener& listener);

  ino_bool popListener(
    Listener& listener);

  ino_size getListenersNum(void) const;

  ino_bool init(
    const ino_interval interval_ms=INO_HANDLER_TIMER_INTERVAL_MS);

  ino_bool loop(void);

private:
  void allocateEventQueues(
    const ino_size event_queue_size,
    const ino_size event_queue_num);

  ino_size popEvent(
    const queue_type q);

  ino_size sendEvent(
    const Event& event);

  ino_bool pushQueueEvent(
    const queue_type q,
    const Event& event);

  ino_size processQueueEvent(
    const queue_type q);

  ino_size flushQueue(
    const queue_type q);

  //event_manager_t       m_manager;
  Listener::listener_list m_listeners;

  ino_handle              m_event_queue;
  ino_size                m_event_queue_num;
  Ticker                  m_timer_low;
  ino_flags               m_state;
};

INO_API_DECLARE

INO_API_DECLARE_END

INO_NAMESPACE_END

#endif    /*INO_EVENT_HANDLER_H*/
