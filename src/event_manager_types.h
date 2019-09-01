#ifndef __EVENT_MANAGER_TYPES_H
#define __EVENT_MANAGER_TYPES_H
#pragma once

#include <stdbool.h>
#include <stdint.h>

/***** events *****************************************************************/
typedef uint32_t        event_code_t;
typedef uint32_t        event_param_t;
typedef void*           event_handle_t;
typedef uint32_t        event_time_t;

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

/*! Type for an event listener (a.k.a. callback function). */
typedef void (*event_listener_fn_t)(const event_t* event);

#endif    /*__EVENT_MANAGER_TYPES_H*/
