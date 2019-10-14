#include <stdio.h>
#include <string.h>

#include "InoAssert.h"
#include "ino_rbuffer.h"

/******************************************************************************/
#ifndef RBUFFER_MAP
#define RBUFFER_MAP(_val, _capacity) \
  rbuffer_map_fast_((_val), (_capacity))
#endif  /*RBUFFER_MAP*/

#define RBUFFER_INC(_val, _capacity) \
  RBUFFER_MAP((_val)+1, (_capacity)<<1)

#define RBUFFER_CLEAR(_b) \
  { (_b)->write_index = (_b)->read_index = 0x0; }

#define RBUFFER_IS_EMPTY(_b) \
  ((_b)->write_index == (_b)->read_index)

#define RBUFFER_IS_FULL(_b) \
  ((_b)->capacity<=RBUFFER_GET_SIZE(_b))

#define RBUFFER_GET_SIZE(_b) \
  rbuffer_get_size_(_b)

#define RBUFFER_SLOT(_b, _pos) \
  ( (_b)->data+((_pos)*(_b)->stride) )


#define RBUFFER_ASSERT_SANITY(_b) \
  INO_ASSERT(((ino_ptr_const)NULL!=((ino_ptr_const)_b)) && ((_b)->data) ) \
  INO_ASSERT(((_b)->stride>0) && ((_b)->capacity>0)) \
  INO_ASSERT(((_b)->write_index<2*((_b)->capacity))) \
  INO_ASSERT(((_b)->read_index<2*((_b)->capacity)))

/******************************************************************************/
/** Private Routines                                                         **/
INO_DECLARE_STATIC
ino_u32 rbuffer_get_size_(
  const ino_rbuffer* b)
{
  if (!b) return 0;
  RBUFFER_ASSERT_SANITY(b)
  const ino_u32 size = (b->write_index < b->read_index)
    ? (((ino_u32)b->capacity<<1) - b->read_index + b->write_index)
    : (ino_u32)(b->write_index - b->read_index);

  INO_ASSERT(size<=b->capacity)

  return size;
}

#ifdef HAS_INO_BUILTIN_TESTS

INO_DECLARE_STATIC
ino_rbuffer_idx rbuffer_map_(
  const ino_u32 idx,
  const ino_u32 capacity)
{
  const ino_rbuffer_idx out = (ino_rbuffer_idx)( idx % capacity );
  INO_ASSERT(out<capacity)
  return out;
}

#endif

INO_DECLARE_STATIC
ino_rbuffer_idx rbuffer_map_fast_(
  const ino_u32 idx,
  const ino_u32 capacity)
{
  INO_ASSERT(idx<2*capacity)
  const ino_rbuffer_idx out = (idx<capacity) ? (idx) : (idx-capacity);
  INO_ASSERT(out<capacity)
  return out;
}

INO_DECLARE_STATIC
ino_u32 rbuffer_sanity_check_(
  const ino_rbuffer* b)
{
  if (b && b->data && (b->stride>0) && (b->capacity>0))
  {
    RBUFFER_ASSERT_SANITY(b)
    return (b->stride*b->capacity);
  }
  return 0;
}

INO_DECLARE_STATIC
ino_bool rbuffer_push_(
  ino_rbuffer* b,
  const ino_handle elem)
{
  RBUFFER_ASSERT_SANITY(b)

  if ( elem ) {
    const ino_u32 slot = RBUFFER_MAP(b->write_index, b->capacity);
    const ino_handle dst = RBUFFER_SLOT(b, slot);
    memcpy(dst, elem, b->stride);
  }
  b->write_index = RBUFFER_INC(b->write_index, b->capacity);

  RBUFFER_ASSERT_SANITY(b)

  return true;
}

INO_DECLARE_STATIC
ino_bool rbuffer_pop_(
  ino_rbuffer* b,
  ino_handle elem)
{
  RBUFFER_ASSERT_SANITY(b)

  if ( elem ) {
    const ino_u32 slot = RBUFFER_MAP(b->read_index, b->capacity);
    const ino_handle src = RBUFFER_SLOT(b, slot);
    memcpy(elem, src, b->stride);
  }

  b->read_index = RBUFFER_INC(b->read_index, b->capacity);

  RBUFFER_ASSERT_SANITY(b)

  return true;
}

/******************************************************************************/
/** Public APIs                                                              **/
INO_API_ENTRY
ino_u32 ino_rbuffer_init(
    ino_rbuffer* b,
    const ino_rbuffer_idx capacity,
    const ino_u16 stride,
    const ino_ptr data)
{
  if ( !(b && data && (capacity>0) && (stride>0)) ) { return 0; }
  const ino_u32 size = stride * capacity;

  b->data     = data;
  b->stride   = stride;
  b->capacity = capacity;
  RBUFFER_CLEAR(b)

  return size;
}

INO_API_ENTRY
void ino_rbuffer_clear(
  ino_rbuffer* b)
{
  if ( !b ) { return; }
  RBUFFER_CLEAR(b)
}

INO_API_ENTRY
ino_u16 ino_rbuffer_get_stride(
  const ino_rbuffer* b)
{
  return (b) ? b->stride : 0;
}

INO_API_ENTRY
ino_u32 ino_rbuffer_get_capacity(
  const ino_rbuffer* b)
{
  return (b) ? b->capacity : 0;
}

INO_API_ENTRY
ino_u32 ino_rbuffer_get_size(
  const ino_rbuffer* b)
{
  return (b) ? RBUFFER_GET_SIZE(b) : 0;
}

INO_API_ENTRY
ino_u32 ino_rbuffer_get_free_size(
  const ino_rbuffer* b)
{
  return (b) ? (b->capacity - RBUFFER_GET_SIZE(b)) : 0;
}

INO_API_ENTRY
ino_bool ino_rbuffer_is_empty(
  const ino_rbuffer* b)
{
  return (b) ? RBUFFER_IS_EMPTY(b) : true;
}

INO_API_ENTRY
ino_bool ino_rbuffer_is_full(
  const ino_rbuffer* b)
{
  return (b) ? RBUFFER_IS_FULL(b) : false;
}

INO_API_ENTRY
ino_bool ino_rbuffer_push_force(
  ino_rbuffer* b,
  const ino_handle elem)
{
  if ( !b ) { return false; }
  RBUFFER_ASSERT_SANITY(b)

  if ( RBUFFER_IS_FULL(b) ) { rbuffer_pop_(b, INO_HANDLE_NULL); }
  return rbuffer_push_(b, elem);
}

INO_API_ENTRY
ino_bool ino_rbuffer_push(
  ino_rbuffer* b,
  const ino_handle elem)
{
  if ( !b ) { return false; }
  RBUFFER_ASSERT_SANITY(b)

  if ( RBUFFER_IS_FULL(b) ) { return false; }
  return rbuffer_push_(b, elem);
}

INO_API_ENTRY
ino_bool ino_rbuffer_pop(
  ino_rbuffer* b,
  ino_handle elem)
{
  if ( !b ) { return false; }
  RBUFFER_ASSERT_SANITY(b)

  if ( RBUFFER_IS_EMPTY(b) ) { return false; }
  return rbuffer_pop_(b, elem);
}

INO_API_ENTRY
ino_handle ino_rbuffer_get_base_address(
  ino_rbuffer* b)
{
  if ( !b ) { return INO_HANDLE_NULL; }
  RBUFFER_ASSERT_SANITY(b)
  return (ino_handle)RBUFFER_SLOT(b, 0x0);
}

INO_API_ENTRY
ino_handle ino_rbuffer_get_end_address(
  ino_rbuffer* b)
{
  if ( !b ) { return INO_HANDLE_NULL; }
  RBUFFER_ASSERT_SANITY(b)
  return (ino_handle)RBUFFER_SLOT(b, b->capacity);
}

INO_API_ENTRY
ino_handle ino_rbuffer_get(
  ino_rbuffer* b,
  const ino_u32 idx)
{
  RBUFFER_ASSERT_SANITY(b)

  if ( !(b && (idx>=RBUFFER_GET_SIZE(b))) ) { return INO_HANDLE_NULL; }

  const ino_u32 pos = RBUFFER_MAP(b->read_index+idx, b->capacity<<1);

  return (ino_handle)RBUFFER_SLOT(b, RBUFFER_MAP(pos, b->capacity));
}

/*
void dump_buffer_(
  ino_rbuffer* b,
  const ino_bool colored)
{
  const ino_u32 ridx = RBUFFER_MAP(b->read_index, b->capacity);
  const ino_u32 widx = RBUFFER_MAP(b->write_index, b->capacity);
  const ino_u32 size = RBUFFER_GET_SIZE(b);

  ino_float* ptr = (ino_float*)b->data;
  printf("   buffer: [ ");
  for ( ino_u32 i=0; i<b->capacity; i++) {
    const ino_bool cl = (i<widx) || ((i>=ridx)&&(i<INO_MIN(ridx+size, b->capacity)));
    if (colored && cl) {
      printf(INO_COLOR_GREEN "%f, " INO_COLOR_OFF, *ptr++);
    } else {
      printf("%f, ", *ptr++);
    }
  }
  printf("]" INO_CR);
}
*/

INO_API_ENTRY
ino_handle ino_rbuffer_linearize(
  ino_rbuffer* b,
  ino_handle temp_swap)
{
  if ( !(b && temp_swap) ) { return INO_HANDLE_NULL; }
  RBUFFER_ASSERT_SANITY(b)

  const ino_u16 stride = b->stride;
  const ino_u32 size = RBUFFER_GET_SIZE(b);
  const ino_rbuffer_idx read_index  = RBUFFER_MAP(b->read_index, b->capacity);
  ino_rbuffer_idx dst_index   = RBUFFER_MAP(b->write_index, b->capacity);
  
  //printf("ino_rbuffer_linearize: size(%u) read_idx(%u)[%u] write_idx(%u)[%u] temp_swap(%p)\n", size, b->read_index, read_index, b->write_index, dst_index, temp_swap);
  //dump_buffer_(b, true);

  if ( !(temp_swap && (size>0) && (stride>0)) ) { return INO_HANDLE_NULL; }

  /* Already linearized */
  if ( (read_index<dst_index) || (0==dst_index) ) {
    return RBUFFER_SLOT(b, read_index);
  }

  const ino_ptr src   = b->data;
  const ino_ptr dst   = RBUFFER_SLOT(b, b->capacity-1);
  const ino_u32 chunk_size = (b->capacity-1) * stride;

  b->read_index  -= dst_index;
  b->write_index -= dst_index;

  //printf("\n   Now Looping: \n");
  //printf("   %u-%u \n\n", dst_index, b->capacity-dst_index);

  for ( ; dst_index>0; ) {
    memcpy(temp_swap, src, stride);
    memmove(src, src+stride, chunk_size);
    memcpy(dst, temp_swap, stride);

    dst_index--;

    //dump_buffer_(b, false);
  }

  //printf("\nFinal Buffer read_idx(%u) write_idx(%u) size(%u): \n", b->read_index, b->write_index, RBUFFER_GET_SIZE(b));
  //dump_buffer_(b, true);

  INO_ASSERT(size==RBUFFER_GET_SIZE(b))

  return RBUFFER_SLOT(b, b->read_index);
}

INO_API_ENTRY
ino_bool ino_rbuffer_clone(
  ino_rbuffer* dst,
  const ino_rbuffer* src)
{
  if ( !(src && dst) ) { return false; }
  const ino_u32 src_size = rbuffer_sanity_check_(src);
  if ( src_size>0 ) {
    const ino_u32 dst_size = rbuffer_sanity_check_(dst);

    if (dst_size>=src_size) {
      const ino_handle dst_ptr = dst->data;
      *dst = *src;
      dst->data = dst_ptr;
      memcpy(dst->data, src->data, src_size);
      return true;
    }
  }
  return false;
}

#ifdef HAS_INO_BUILTIN_TESTS

INO_API_ENTRY
int ino_builtin_tests_ino_rbuffer(
  int argc, char* argv[])
{
  const ino_u32 max_capacity = INO_RBUFFER_MAX_CAPACITY;
  INO_RBUFFER_DECLARE(b, 15, INO_ARRAY_STATIC(15, ino_float))

  int failed = 0;
  ino_u32 idx0 = 0, idx1 = 0, idx2 = 0;
  ino_u32 didx0 = 0, didx1 = 0, didx2 = 0;
  ino_float elem;

  RBUFFER_ASSERT_SANITY(&b)

  for (ino_u32 i=0; i<=2*max_capacity; i++) {
    idx0 = rbuffer_map_(idx0, b.capacity);
    idx1 = rbuffer_map_fast_(idx1, b.capacity);
    idx2 = RBUFFER_MAP(idx2, b.capacity);
    failed += (idx0!=idx1);
    failed += (idx0!=idx2);
    idx0++; idx1++; idx2++;

    didx0 = rbuffer_map_(didx0, 2*b.capacity);
    didx1 = rbuffer_map_fast_(didx1, 2*b.capacity);
    didx2 = RBUFFER_MAP(didx2, 2*b.capacity);
    failed += (didx0!=didx1);
    failed += (didx0!=didx2);
    didx0++; didx1++; didx2++;
  }

  for ( ino_i32 i=0; i<b.capacity+5; i++) {
    elem = i;
    ino_rbuffer_push_force(&b, &elem);
  }
  ino_rbuffer_pop(&b, INO_HANDLE_NULL);

  ino_rbuffer_linearize(&b, &elem);

  return failed;
}

#endif    /*HAS_INO_BUILTIN_TESTS*/

#undef RBUFFER_MAP
#undef RBUFFER_INC
#undef RBUFFER_CLEAR
#undef RBUFFER_IS_EMPTY
#undef RBUFFER_IS_FULL
#undef RBUFFER_GET_SIZE
#undef RBUFFER_ASSERT_SANITY
