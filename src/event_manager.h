/**
 * Simple event manager module.
 * This file has been derived by an earlier event handling system developed
 * for Arduino by Igor Mikolic-Torreira 
 * (https://github.com/igormiktor/arduino-EventManager)
 */


#ifndef __EVENT_MANAGER_H
#define __EVENT_MANAGER_H
#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "event_manager_types.h"

/*! Maximun number of listeners. Adjust as appropriate for your application. */
#define MAX_NUM_OF_LISTENERS    (24)

/*! Maximun number of events in a queue. */
#define MAX_EVENT_QUEUE_SIZE    (24)

/*! Number of event queues in the Event Manager. */
#define NUM_OF_QUEUES           (2)

#define HIGHER_PRIORITY_QUEUE   (0)
#define LOWER_PRIORITY_QUEUE    (NUM_OF_QUEUES-1)

/***** event queues ***********************************************************/

/*!
 * Structure implementing an event queue.
 */

typedef struct event_queue_t_ {
  /*! Array of events in the queue. */
  event_t events[MAX_EVENT_QUEUE_SIZE];

  /*! Index of event queue head. */
  uint32_t head;

  /*! Index of event queue tail. */
  uint32_t tail;
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
} event_listener_t;

/*!
 * Structure implementing a set of event listeners.
 */

typedef struct event_listeners_t_ {
  /*! Set of event listeners. */
  event_listener_t listener[MAX_NUM_OF_LISTENERS+1];

  /*! Current number of event listeners. */
  uint32_t num;

} event_listeners_t;


/***** event manager **********************************************************/

typedef struct event_manager_t_ {
  /*! Queues in the Event Manager. */
  event_queue_t queue[NUM_OF_QUEUES];
    
  /*! Listeners registered in the Event Manager. */
  event_listeners_t listeners;
} event_manager_t;


/***** routines ***************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/*! initialize an event manager */
void event_manager_init(
  event_manager_t *manager,
  const event_listener_fn_t default_listener);

/*! reset an event manager without processing queues */
void event_manager_reset(
  event_manager_t *manager);

/*! bind a listener to an event manager */
bool event_manager_bind_listener(
  event_manager_t *manager,
  const event_code_t event_codes,
  const event_listener_fn_t listener);

/*! unbind a listener to an event manager */
bool event_manager_unbind_listener(
  event_manager_t *manager,
  const event_listener_fn_t listener);

/*! check if one of the event queues of an event manager is empty */
bool event_manager_queue_is_empty(
  const event_manager_t *manager,
  const uint32_t q);

/*! check if one of the event queues of an event manager is full */
bool event_manager_queue_is_full(
  const event_manager_t *manager,
  const uint32_t q);

/*! get the number of events contained in one of the event queues of an event manager */
uint32_t event_manager_get_num_of_events(
  const event_manager_t *manager,
  const uint32_t q);

/*! push an event to one of the event queues of an event manager */
bool event_manager_push_event(
  event_manager_t *manager,
  const uint32_t q,
  const event_t* event);

/*! pop the first event contained in one of the event queues of an event manager */
uint32_t event_manager_pop_event(
  event_manager_t *manager,
  const uint32_t q);

/*!
 * Process all the event queues present within event manager.
 * if higher_priority_only is true it process only events int the 1st higher 
 * priority queue found 
 */
uint32_t event_manager_flush(
  event_manager_t *manager,
  const bool higher_priority_only);

#ifdef __cplusplus
}
#endif

#endif    /*__EVENT_MANAGER_H*/
