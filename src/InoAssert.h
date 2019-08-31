#ifndef __INO_ASSERT_H__
#define __INO_ASSERT_H__
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

#endif      /*__INO_ASSERT_H__*/
