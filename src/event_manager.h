/**
 * Simple event manager module.
 * This file has been derived by an earlier event handling system developed
 * for Arduino by Igor Mikolic-Torreira 
 * (https://github.com/igormiktor/arduino-EventManager)
 */
#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H
#pragma once

#include "ino_dlist.h"

/*! Maximun number of events in a queue. */
#define MAX_EVENT_QUEUE_SIZE    (24)

/*! Number of event queues in the Event Manager. */
#define NUM_OF_QUEUES           (2)

#define HIGH_PRIORITY_QUEUE     (0)
#define LOW_PRIORITY_QUEUE      (NUM_OF_QUEUES-1)

INO_NAMESPACE

/***** events *****************************************************************/
typedef ino_u32        event_code_t;
typedef ino_handle     event_handle_t;
typedef ino_u32        event_time_t;

/*!
 * Structure implementing an event.
 */
typedef struct event_t_ {
  /*! Event code. */
  event_code_t      code;

  /*! Opaque cookie handle passed by the event to the listener. */
  event_handle_t    cookie;

  /*! Timestamp when the event was generated. */
  event_time_t      timestamp;
} event_t;

/***** event listeners ********************************************************/
typedef ino_handle     listener_handle_t;

/*! Type for an event listener (a.k.a. callback function). */
typedef void (*event_listener_fn_t)(
  const event_t* event, listener_handle_t cookie);

/***** event queues ***********************************************************/

/*!
 * Structure implementing an event queue.
 */
typedef struct event_queue_t_ {
  /*! Array of events in the queue. */
  event_t events[MAX_EVENT_QUEUE_SIZE];

  /*! Index of event queue head. */
  ino_u32 head;

  /*! Index of event queue tail. */
  ino_u32 tail;
} event_queue_t;


/***** event listeners ********************************************************/

/*!
 * Structure implementing an event listener.
 */
typedef struct event_listener_t_ {
  /*! Mask of all the event codes associated to this listener. */
  event_code_t event_codes;

  /*! Listener function. */
  event_listener_fn_t callback;
  listener_handle_t   cookie;

  ino_dlist           list;
} event_listener_t;

/***** event manager **********************************************************/

typedef struct event_manager_t_ {
  /*! Queues in the Event Manager. */
  event_queue_t queue[NUM_OF_QUEUES];
    
  /*! Listeners list registered in the Event Manager. */
  event_listener_t listeners;

} event_manager_t;


/***** routines ***************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/*! initialize an event manager */
void event_manager_init(
  event_manager_t* manager,
  const event_listener_fn_t default_listener,
  listener_handle_t listener_cookie);

/*! reset an event manager without processing queues */
void event_manager_reset(
  event_manager_t* manager);

/*! bind a listener to an event manager */
ino_bool event_manager_bind_listener(
  event_manager_t* manager,
  event_listener_t* listener);

/*! unbind a listener to an event manager */
ino_bool event_manager_unbind_listener(
  event_manager_t* manager,
  event_listener_t* listener);

/*! check if one of the event queues of an event manager is empty */
ino_bool event_manager_queue_is_empty(
  const event_manager_t* manager,
  const ino_u32 q);

/*! check if one of the event queues of an event manager is full */
ino_bool event_manager_queue_is_full(
  const event_manager_t* manager,
  const ino_u32 q);

/*! get the number of events contained in one of the event queues of an event manager */
ino_u32 event_manager_get_num_of_events(
  const event_manager_t* manager,
  const ino_u32 q);

/*! push an event to one of the event queues of an event manager */
ino_bool event_manager_push_event(
  event_manager_t* manager,
  const ino_u32 q,
  const event_t* event);

/*! pop the first event contained in one of the event queues of an event manager */
ino_u32 event_manager_pop_event(
  event_manager_t* manager,
  const ino_u32 q);

/*!
 * Process all the event queues present within event manager.
 * if higher_priority_only is true it process only events int the 1st higher 
 * priority queue found 
 */
ino_u32 event_manager_flush(
  event_manager_t* manager,
  const ino_bool higher_priority_only);

#ifdef __cplusplus
}
#endif

INO_NAMESPACE_END

#endif    /*EVENT_MANAGER_H*/
