#include <stdio.h>

#include "ino_dlist.h"

typedef struct {
  ino_u32    id;
  ino_dlist  list;
} my_elem;

int main(int argc, char* argv[])
{
  my_elem array[64];
  INO_DLIST_DECLARE(head)

  for (ino_u32 i=0; i<INO_ARRAY_COUNT(array); i++) {
    array[i].id = i;
    INO_DLIST_INIT(&array[i].list)

    //INO_DLIST_PUSH_FRONT(&array[i].list, &head)
    INO_DLIST_PUSH_BACK(&array[i].list, &head)
  }

  printf("## Current list size(%u):\n", INO_DLIST_SIZE(&head));
  INO_DLIST_FOR_EACH_OBJ_NEXT_SAFE(item, &head, my_elem, list)
  //INO_DLIST_FOR_EACH_OBJ_PREV_SAFE(item, &head, my_elem, list)
  {
    INO_DLIST_OBJ_REMOVE(item, list)

    printf("      elem: id(%u) next(%p) prev(%p)\n", item->id, item->list.next, item->list.prev);
  }

  printf("## Current list size(%u):\n", INO_DLIST_SIZE(&head));

  return 0;
}
