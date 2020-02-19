#ifndef INO_RBUFFER_H
#define INO_RBUFFER_H
#include "InoTypes.h"


#define INO_RBUFFER_MAX_CAPACITY \
  ((ino_rbuffer_idx)(0x1<<((8*sizeof(ino_rbuffer_idx))-1)))

#define INO_RBUFFER_INIT(_capacity, _data_ptr) { \
  .data        = INO_PTR(_data_ptr), \
  .stride      = sizeof((_data_ptr)[0]), \
  .capacity    = (_capacity), \
  .read_index  = 0x0, \
  .write_index = 0x0 \
}

#define INO_RBUFFER_DECLARE(_name, _capacity, _data_ptr) \
  ino_rbuffer _name = INO_RBUFFER_INIT(_capacity, _data_ptr);


/*** Ring Buffer Typedefs ******************************************************/
typedef ino_u16   ino_rbuffer_idx;

typedef struct ino_rbuffer_s {
  ino_ptr             data;
  ino_u16             stride;
  ino_rbuffer_idx     capacity;
  ino_rbuffer_idx     read_index;
  ino_rbuffer_idx     write_index;
} ino_rbuffer;


/*** Public APIs Declarations *************************************************/

INO_API_DECLARE

INO_API_ENTRY
ino_u32 ino_rbuffer_init(
  ino_rbuffer* b,
  const ino_rbuffer_idx capacity,
  const ino_u16 stride,
  const ino_ptr data);

INO_API_ENTRY
void ino_rbuffer_clear(
  ino_rbuffer* b);

INO_API_ENTRY
ino_u16 ino_rbuffer_get_stride(
  const ino_rbuffer* b);

INO_API_ENTRY
ino_u32 ino_rbuffer_get_capacity(
  const ino_rbuffer* b);

INO_API_ENTRY
ino_u32 ino_rbuffer_get_size(
  const ino_rbuffer* b);

INO_API_ENTRY
ino_u32 ino_rbuffer_get_free_size(
  const ino_rbuffer* b);

INO_API_ENTRY
ino_bool ino_rbuffer_is_empty(
  const ino_rbuffer* b);

INO_API_ENTRY
ino_bool ino_rbuffer_is_full(
  const ino_rbuffer* b);

INO_API_ENTRY
ino_bool ino_rbuffer_push_force(
  ino_rbuffer* b,
  const ino_handle elem);

INO_API_ENTRY
ino_bool  ino_rbuffer_push(
  ino_rbuffer* b,
  const ino_handle elem);

INO_API_ENTRY
ino_bool ino_rbuffer_pop(
  ino_rbuffer* b,
  ino_handle elem);

INO_API_ENTRY
ino_handle ino_rbuffer_get_base_address(
  ino_rbuffer* b);

INO_API_ENTRY
ino_handle ino_rbuffer_get_end_address(
  ino_rbuffer* b);

INO_API_ENTRY
ino_handle ino_rbuffer_get(
  ino_rbuffer* b,
  const ino_u32 idx);

INO_API_ENTRY
ino_handle ino_rbuffer_linearize(
  ino_rbuffer* b,
  ino_handle temp_swap);

INO_API_ENTRY
ino_bool ino_rbuffer_clone(
  ino_rbuffer* dst,
  const ino_rbuffer* src);

#ifdef HAS_INO_BUILTIN_TESTS
INO_API_ENTRY
int ino_builtin_tests_ino_rbuffer(
  int argc, char* argv[]);
#endif  /*HAS_INO_BUILTIN_TESTS*/


INO_API_DECLARE_END

#endif    /*INO_RBUFFER_H*/
