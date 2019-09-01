#ifndef __INO_DLIST_H
#define __INO_DLIST_H
#include "InoTypes.h"
#include "InoAssert.h"

/*! Initialize a list setting its prev and next fields */
#define INO_DLIST_SET(item_, next_, prev_) \
  (item_)->next = (next_); (item_)->prev = (prev_);

/*! Initialize an empty list setting prev and next fields to point to itself */
#define INO_DLIST_DECLARE(item_) \
  ino_dlist item_ = { .next = &(item_), .prev = &(item_) };

/*! Initialize an empty list setting prev and next fields to point to itself */
#define INO_DLIST_INIT(item_) \
  INO_DLIST_SET(item_, item_, item_)

/*! Insert an element into a list */
#define INO_DLIST_INSERT(item_, next_, prev_) \
  ino_dlist_insert(item_, next_, prev_);

/*! Insert an element before head in the list */
#define INO_DLIST_PUSH_FRONT(item_, head_) \
  INO_DLIST_INSERT(item_, head_, (head_)->prev)

/*! Insert an element after head on the list */
#define INO_DLIST_PUSH_BACK(item_, head_) \
  INO_DLIST_INSERT(item_, (head_)->next, head_)

/*! Check if the list is empty */
#define INO_DLIST_IS_EMPTY(item_) \
  ( (item_)==(item_)->next )

/**** END DOUBLE LINKED LIST MACROS SECTION ***********************************/
/*
#include <stddef.h>
#define INO_STRUCT_OFFSET(type_, field_) \
  ( (ino_u32)(offsetof(type_,field_)) )
*/

/*! Computes offset in byte of a  named "member_" into a container of type 
 * "type_" */
#define INO_STRUCT_OFFSET(type_, member_) \
  ( (ino_u32)((ino_uptr)&(((type_*)0)->member_)) )

/*! Cast the pointer as a type_* pointer */
#define INO_CONTAINER_TYPE(type_, ptr_) \
  ( (type_*)(ptr_) )

/*! Get the start address of the container datastruct */
#define INO_CONTAINER_PTR(ptr_, offset_) \
  ( INO_PTR(ptr_)-(offset_) )

/*! Get start address of the container "type_" from the pointer to the list 
 * ino_dlist element named "member_" */
#define INO_DLIST_CONTAINER(ptr_, type_, member_) \
  INO_CONTAINER_TYPE( type_, \
    INO_CONTAINER_PTR(ptr_, INO_STRUCT_OFFSET(type_, member_)) )

/*! Get the size of a list */
#define INO_DLIST_SIZE(head_) \
  ino_dlist_size(head_)

/*! For loop that iterates forward on the ino_dlist elements */
#define INO_DLIST_FOR_EACH_NEXT(item_, head_) \
  for ( ino_dlist *item_=(head_)->next; item_!=(head_); item_=item_->next )

/*! For loop that iterates backward on the ino_dlist elements */
#define INO_DLIST_FOR_EACH_PREV(item_, head_) \
  for ( ino_dlist *item_=(head_)->prev; item_!=(head_); item_=item_->prev )

/*! For loop that safely iterates forward on the ino_dlist elements */
#define INO_DLIST_FOR_EACH_NEXT_SAFE(item_, head_) \
  for ( ino_dlist *item_=(head_)->next, *n_=item_->next; \
        item_!=(head_); item_=n_, n_=n_->next )

/*! For loop that safely iterates backward on the ino_dlist elements */
#define INO_DLIST_FOR_EACH_PREV_SAFE(item_, head_) \
  for ( ino_dlist *item_=(head_)->prev, *p_=item_->prev; item_!=(head_); \
        item_=p_, p_=p_->prev )

/*! Remove an element from the list */
#define INO_DLIST_REMOVE(next_, prev_) \
  { (next_)->prev = (prev_); (prev_)->next = (next_); }

/*! Concatenate list head_ in position where_ of another list */
#define INO_DLIST_SPLICE(head_, where_) \
  ino_dlist_splice(head_, where_);

/*! Remove an element from the list using the pointer of the container */
#define INO_DLIST_OBJ_REMOVE(item_, member_) \
  INO_DLIST_REMOVE((item_)->member_.next, (item_)->member_.prev)

/*! For loop that iterates forward on the container elements */
#define INO_DLIST_FOR_EACH_OBJ_NEXT(obj_, head_, type_, member_) \
  for ( type_ *obj_=INO_DLIST_CONTAINER((head_)->next, type_, member_); \
        &obj_->member_!=(head_); \
        obj_=INO_DLIST_CONTAINER(obj_->member_.next, type_, member_ ) )

/*! For loop that iterates backward on the container elements */
#define INO_DLIST_FOR_EACH_OBJ_PREV(obj_, head_, type_, member_) \
  for ( type_ *obj_=INO_DLIST_CONTAINER((head_)->prev, type_, member_); \
        &obj_->member_!=(head_); \
        obj_=INO_DLIST_CONTAINER(obj_->member_.prev, type_, member_ ) )

/*! For loop that safely iterates forward on the container elements */
#define INO_DLIST_FOR_EACH_OBJ_NEXT_SAFE(obj_, head_, type_, member_) \
  for ( type_ *obj_=INO_DLIST_CONTAINER((head_)->next, type_, member_), \
        *n_= INO_DLIST_CONTAINER(obj_->member_.next, type_, member_); \
        &obj_->member_!=(head_); \
        obj_=n_, n_=INO_DLIST_CONTAINER(n_->member_.next, type_, member_ ) )

/*! For loop that safely iterates backward on the container elements */
#define INO_DLIST_FOR_EACH_OBJ_PREV_SAFE(obj_, head_, type_, member_) \
  for ( type_ *obj_=INO_DLIST_CONTAINER((head_)->prev, type_, member_), \
        *p_= INO_DLIST_CONTAINER(obj_->member_.prev, type_, member_); \
        &obj_->member_!=(head_); \
        obj_=p_, p_=INO_DLIST_CONTAINER(p_->member_.prev, type_, member_ ) )

/*!
 * @typedef ino_dlist
 * @ingroup core_lists
 * @brief Implementation of a double linked datastracture to handle generic
 * double-linked lists elements
 */
typedef struct ino_dlist_ {
  struct ino_dlist_*     next; /*!< pointer to the next element of the list */
  struct ino_dlist_*     prev; /*!< pointer to the previous element of the list */
} ino_dlist;

/*!
 * @brief Insert an item inside the list
 * @details this routine inserts an item in a list between the next and prev 
 * element.
 * @ingroup core_lists
 * @param[in/out] item a pointer to the @ref ino_dlist datastructure to be inserted
 * @param[in/out] next a pointer to the next element in the list
 * @param[in/out] prev a pointer to the previous element in the list
 */
INO_DECLARE_STATIC
void ino_dlist_insert(ino_dlist* item, ino_dlist* next, ino_dlist* prev)
{
  INO_ASSERT(item && next && prev)
  item->next = next;
  item->prev = prev;
  next->prev = item;
  prev->next = item;
}

/*!
 * @brief Insert a list head into another list into position where 
 * @details this routine inserts an full list in position where on another
 * list
 * @ingroup core_lists
 * @param[in/out] head a pointer to the list to be inserted
 * @param[in/out] where a pointer to the position in the 2nd list where to insert the
 * 1st list
 */
INO_DECLARE_STATIC
void ino_dlist_splice(ino_dlist* head, ino_dlist* where)
{
  INO_ASSERT(head && where)
  if ( INO_DLIST_IS_EMPTY(head) ) return;

  ino_dlist* first = head->next;
  ino_dlist* last  = head->prev;
  ino_dlist* where_last = where->next;

  first->prev      = where;
  where->next      = first;

  last->next       = where_last;
  where_last->prev = last;

  INO_DLIST_INIT(head)
}

/*!
 * @brief Get the number of elements in the list
 * @ingroup core_lists
 * @param[in] head a pointer to the list
 */
INO_DECLARE_STATIC
ino_u32 ino_dlist_size(const ino_dlist* head)
{
  INO_ASSERT(head)
  ino_u32 size = 0;
  INO_DLIST_FOR_EACH_NEXT(item, head) { size++; }
  return size;
}

#endif    /*__INO_DLIST_H*/
