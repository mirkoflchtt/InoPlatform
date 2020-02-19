#ifndef INO_ASSERT_H
#define INO_ASSERT_H
#include "InoTypes.h"

#ifdef MY_INO_ASSERT
#include <assert.h>
#define INO_ASSERT(expr) \
  INO_WRAP_FUNC(assert(expr);)
#else
#define INO_ASSERT(expr) \
  INO_WRAP_FUNC(INO_NOP)
#endif

INO_NAMESPACE

INO_NAMESPACE_END

#endif      /*INO_ASSERT_H*/
