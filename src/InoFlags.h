#ifndef INO_FLAGS_H
#define INO_FLAGS_H
#include "InoTypes.h"

#define INO_FLAG_CLEAR(flags)          { flags = INO_FLAG(ino::FLAG_NONE); }
#define INO_FLAG_SET(flags, mask)      { flags |=   INO_FLAG(mask); }
#define INO_FLAG_UNSET(flags, mask)    { flags &= INO_FLAG(~(mask)); }
#define INO_FLAG_GET(flags, mask)      (INO_FLAG(flags) & INO_FLAG(mask))

#define INO_FLAG(flag)                  ((ino_flags)(flag))

INO_NAMESPACE

enum { FLAG_NONE = 0x0 };

typedef ino_i32       ino_flags;

INO_NAMESPACE_END

#endif    /*INO_FLAGS_H*/
