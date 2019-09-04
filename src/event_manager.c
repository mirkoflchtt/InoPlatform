//#include <stdio.h>
#include <stdlib.h>

#include "InoAssert.h"
#include "InoHalAdapter.h"
#include "event_manager.h"

/*!
 * A dummy listener func called when no listener is set
 */
static inline
void dummy_listener(
  const event_t* event,
  listener_handle_t cookie)
{
  (void)(event);
  (void)(cookie);
}

/***** event queues ***********************************************************/

/*!
 * Initialize an event queue.
 *
 * \param[out] queue is the pointer to the event queue that must be initialized
 */

static inline
void _queue_init(
  event_queue_t* queue)
{
  queue->head = queue->tail = ~0x0;
}

static inline
ino_u32 _queue_size(
  const event_queue_t* queue)
{
  INO_ASSERT( queue )
  const ino_bool wrap_around = ( queue->tail<queue->head );
  const ino_u32 size = (wrap_around)
                        ? (((~0x0)-queue->head) + queue->tail + 1)
                        : ( queue->tail - queue->head );
#if 0
  if (wrap_around)
  {
    printf("[event_queue_size] wrap_around!! tail(%08x) head(%08x) size(%u)\r\n",
           queue->tail, queue->head, size);
  }
#endif
  return size;
}

/*!
 * Check if an event queue is empty.
 *
 * \param[in] queue is the pointer to the event queue
 *
 * \retval #true  if event queue \e queue is empty
 * \retval #false otherwise
 */

static inline
ino_bool _queue_is_empty(
  const event_queue_t* queue)
{
  return ( queue->tail==queue->head );
}


/*!
 * Check if an event queue is full.
 *
 * \param[in] queue is the pointer to the event queue
 *
 * \retval #true  if event queue \e queue is full
 * \retval #false otherwise
 */

static inline
ino_bool _queue_is_full(
  const event_queue_t* queue)
{
  return ( MAX_EVENT_QUEUE_SIZE==_queue_size(queue) );
}

/*!
 * Push an event to an event queue.
 *
 * \param[in,out] queue is the pointer to the event queue
 * \param[in]     event is teh pointer to the event to push
 *
 * \retval #true  on success
 * \retval #false otherwise
 */

static
ino_bool _queue_push_event(
  event_queue_t* queue,
  const event_t* event)
{
  INO_ASSERT(queue && event)
  
  disable_interrupts();

  if ( _queue_is_full(queue) ) {
    enable_interrupts();
    return false;
  }

  //printf("[event_queue_push_event] on slot %u\r\n", queue->tail % MAX_EVENT_QUEUE_SIZE);

  queue->events[ queue->tail % MAX_EVENT_QUEUE_SIZE ] = *event;
  queue->tail++;

  enable_interrupts();

  return true;
}


/*!
 * Pop an event from an event queue.
 *
 * \param[in,out] queue  is the pointer to the event queue
 * \param[in]     event  is the pointer to the event
 *
 * \retval #true  on success
 * \retval #false otherwise
 */

static
ino_bool _queue_pop_event(
  event_queue_t* queue,
  event_t* event)
{
  INO_ASSERT(queue && event)

  if ( _queue_is_empty(queue) ) {
    return false;
  }

  disable_interrupts();

  //printf("[event_queue_pop_event] on slot %u\r\n", queue->head % MAX_EVENT_QUEUE_SIZE);

  *event = queue->events[ queue->head % MAX_EVENT_QUEUE_SIZE ];
  queue->head++;

  enable_interrupts();

  return true;
}

/***** event listeners ********************************************************/

/*!
 * Find an empty listener slot
 *
 * \param[in] listeners ia a pointer to listeners
 * \param[in] listener_cb is the pointer to the listener function to search
 */
/*
static inline
event_listener_t* _listeners_find_slot(
  event_listener_t* listeners,
  const event_listener_fn_t listener_cb)
{
  INO_ASSERT(listeners && listener_cb)

  INO_DLIST_FOR_EACH_OBJ_NEXT(l, &listeners->list, event_listener_t, list)
  {
    if (l->callback==listener_cb) {
      return l;
    }
  }
  return NULL;
}
*/

/*!
 * Bind a listener.
 *
 * \param[in,out] listeners   is the pointer to the set of event listeners where the new listener must be registered
 * \param[in]     event_codes is the mask of all event codes associated to the listener that must be registered
 * \param[in]     listener_cb is the pointer to the listener function that must be registered
 */

static
ino_bool _listeners_bind_listener(
  event_listener_t* listeners,
  event_listener_t* item) 
{
  INO_ASSERT(listeners && item)

  if ( !item->callback ) {
    return false;
  }

  INO_DLIST_INIT(&item->list)
  INO_DLIST_PUSH_FRONT(&item->list, &listeners->list)
  
  return true;
}

/*!
 * Unbind a listener.
 *
 * \param[in,out] listeners   is the pointer to the set of event listeners where the new listener must be registered
 * \param[in]     listener    is the pointer to the listener function that must be registered
 */

static
ino_bool _listeners_unbind_listener(
  event_listener_t* listeners,
  event_listener_t* listener) 
{
  INO_ASSERT(listeners && listener)

  INO_UNUSED(listeners)
  /*
  event_listener_t* l = _listeners_find_slot(listeners, listener_cb);
  if ( !l ) {
    return false;
  }
  */

  INO_DLIST_OBJ_REMOVE(listener, list)
  INO_DLIST_INIT(&listener->list)

  return true;
}

/*!
 * Initializes a set of event listeners.
 *
 * \param[out] listeners        is the pointer to the set of event listeners
 * that must be initialized
 * \param[in]  default_listener if not #NULL, is the pointer to the default 
 * listener function
 *
 */

static
void _listeners_init(
  event_listener_t* listeners,
  const event_listener_fn_t default_listener,
  listener_handle_t listener_cookie)
{
  INO_ASSERT(listeners)

  /* Bind default listener on reserved slot 0 */
  listeners->event_codes = 0x0;
  listeners->callback = 
    (default_listener) ? default_listener : dummy_listener;
  listeners->cookie = (listener_cookie) ? listener_cookie : NULL;
  INO_DLIST_INIT(&listeners->list)
}


/*!
 * Send an event to one or more listeners.
 *
 * \param[in,out] listeners  is the pointer to the set of event listeners containing the listeners where the event must be sent
 * \param[in]     event is the pointer to the event to send
 *
 * \return the number of listeners contained in \e listeners and associated to the event having code \e event_code
 */

static
ino_u32 _listeners_send_event(
  const event_listener_t* listeners,
  const event_t* event)
{
  INO_ASSERT(listeners && event)
  
  ino_u32 count = 0;

  INO_DLIST_FOR_EACH_OBJ_NEXT(l, &listeners->list, event_listener_t, list)
  {
    const ino_bool trigger = ((event->code & l->event_codes) == event->code);
    //printf("### trigger(%d) event->code(0x%08x) l->codes(0x%08x) AND(0x%08x)\n", trigger, event->code, l->event_codes, (event->code & l->event_codes));

    INO_ASSERT(l->callback)
    INO_ASSERT(l->callback!=dummy_listener)
    if ( trigger ) {
      l->callback(event, l->cookie);
      count++;
    }
  }

  if ( 0==count ) {
    const event_listener_t *l = listeners;
    INO_ASSERT(l->callback)

    l->callback(event, l->cookie);
    count += (l->callback!=dummy_listener);
  }

  return count;
}

/***** event manager **********************************************************/

/*!
 * Process a single event on a queue 
 *
 * \param[in/ut] a pointer to queue to process 
 * \param[in]  listeners a pointer to the listeners to trigger
 */
static
ino_u32 _manager_process_event(
  event_queue_t* queue,
  const event_listener_t* listeners)
{
  INO_ASSERT(queue && listeners)

  event_t event;

  if ( _queue_pop_event(queue, &event) ) {
    return _listeners_send_event(listeners, &event);
  }
  return 0;
}

/*!
 * Initialize an event manager.
 *
 * \param[out] manager          is the pointer to the event manager
 * \param[in]  default_listener if not #NULL, is the pointer to the default listener function
 */

void event_manager_init(
  event_manager_t* manager,
  const event_listener_fn_t default_listener,
  listener_handle_t listener_cookie)
{
  INO_ASSERT(manager)

  event_manager_reset(manager);
  
  _listeners_init(&manager->listeners, default_listener, listener_cookie);
}


/*!
 * Reset an event manager without processing queues
 *
 * \param[in,out] manager     is the pointer to the event manager
 *
 */
void event_manager_reset(
  event_manager_t* manager)
{
  INO_ASSERT(manager)
  
  for ( ino_u32 q = 0; q < NUM_OF_QUEUES; q++ ) {
    _queue_init(&manager->queue[q]);
  }
}

/*!
 * Bind a listener to an event manager.
 *
 * \param[in,out] manager     is the pointer to the event manager
 * \param[in]     event_codes is the mask of all event codes associated to the listener that must be registered
 * \param[in]     listener    is the pointer to the listener function that must be registered
 */

ino_bool event_manager_bind_listener(
  event_manager_t* manager,
  event_listener_t* listener)
{ 
  INO_ASSERT(manager)
  return _listeners_bind_listener(
    &manager->listeners, listener);
}


/*!
 * Unbind a listener from an event manager.
 *
 * \param[in,out] manager     is the pointer to the event manager
 * \param[in]     listener    is the pointer to the listener function that must be registered
 */
ino_bool event_manager_unbind_listener(
  event_manager_t* manager,
  event_listener_t* listener)
{
  return _listeners_unbind_listener(&manager->listeners, listener);
}

/*!
 * Chek if one of the event queues of an event manager is empty.
 *
 * \param[in] manager is the pointer to the event manager
 * \param[in] q       is the index of the event manager queue that must be considered
 *
 * \retval #true  if queue having index \e q of event manager \e event_manager is empty
 * \retval #false otherwise
 */

ino_bool event_manager_queue_is_empty(
  const event_manager_t *manager,
  const ino_u32 q)
{
  INO_ASSERT(manager)
  if (q >= NUM_OF_QUEUES) {
    return false;
  }

  return _queue_is_empty( &manager->queue[q] );
}


/*!
 * Chek if one of the event queues of an event manager is full.
 *
 * \param[in] manager is the pointer to the event manager
 * \param[in] q       is the index of the event manager queue that must be considered
 *
 * \retval #true  if queue having index \e q of event manager \e event_manager is full
 * \retval #false otherwise
 */

ino_bool event_manager_queue_is_full(
  const event_manager_t *manager,
  const ino_u32 q)
{
  INO_ASSERT(manager)
  if (q >= NUM_OF_QUEUES) {
    return false;
  }

  return _queue_is_full(&manager->queue[q]);
}


/*!
 * Get the number of events contained in one of the event queues of an event manager.
 *
 * \param[in] manager is the pointer to the event manager
 * \param[in] q       is the index of the event manager queue that must be considered
 *
 * \return the number of events contained in queue having index \e q of event manager \e event_manager
 */

ino_u32 event_manager_get_num_of_events(
  const event_manager_t *manager,
  const ino_u32 q)
{
  INO_ASSERT(manager)
  if (q >= NUM_OF_QUEUES) {
    return 0;
  }
  return _queue_size(&manager->queue[q]);
}


/*!
 * Push an event to one of the event queues of an event manager.
 *
 * \param[in,out] manager     is the pointer to the event manager
 * \param[in]     q           is the index of the event manager queue that must be considered
 * \param[in]     event is the pointer to the event to push
 *
 * \retval #true  if the event has been correctly pushed to queue having index \e q of event manager \e event_manager
 * \retval #false otherwise
 */

ino_bool event_manager_push_event(
  event_manager_t *manager,
  const ino_u32 q,
  const event_t* event)
{
  INO_ASSERT(manager && event)

  if (q >= NUM_OF_QUEUES) {
    return false;
  }

  if ( _queue_is_full(&manager->queue[q]) ) {
    while ( 0==_manager_process_event(&manager->queue[q], &manager->listeners) );
    INO_ASSERT( !_queue_is_full(&manager->queue[q]) )
    //printf("[event_manager_push_event] :queue was full..\r\n");
  }
  return _queue_push_event(&manager->queue[q], event);
}


/*!
 * Process the first event contained in one of the event q-esima queue of an event manager.
 *
 * \param[in,out] manager is the pointer to the event manager
 * \param[in]     q       is the index of the event manager queue that must be considered
 *
 * \return the number of listeners contained in \e manager and associated to the event having code \e event_code
 */

ino_u32 event_manager_pop_event(
  event_manager_t *manager,
  const ino_u32 q)
{
  INO_ASSERT( manager )

  if (q >= NUM_OF_QUEUES) {
    return 0;
  }

  return _manager_process_event(&manager->queue[q], &manager->listeners);
}

/*!
 * Process all the event queues present within event manager.
 * if higher_priority_only is true it process only events int the 1st higher priority queue found
 *
 * \param[in,out] manager is the pointer to the event manager
 *
 * \param[in] higher_priority_only is a flag for flushing all events from 1st higher priority queue
 * \return the number of listeners contained in \e manager and associated to the event having code \e event_code
 */
ino_u32 event_manager_flush(
  event_manager_t *manager,
  const ino_bool higher_priority_only)
{
  ino_u32 count = 0;

#if ( HIGHER_PRIORITY_QUEUE>LOWER_PRIORITY_QUEUE )
  for ( ino_i32 q=HIGH_PRIORITY_QUEUE; q>=LOW_PRIORITY_QUEUE; q-- )
#else 
  for ( ino_i32 q=HIGH_PRIORITY_QUEUE; q<=LOW_PRIORITY_QUEUE; q++ )
#endif
  {
    ino_u32 processed;
    while ( (processed = _manager_process_event(&manager->queue[q], &manager->listeners))>0 ) {
      count += processed;
    }
      
    if ( higher_priority_only && (count>0) ) {
      break;
    }
  }
  
  return count;
}
