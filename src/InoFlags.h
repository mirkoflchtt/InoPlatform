#ifndef INO_FLAGS_H
#define INO_FLAGS_H
#include "InoTypes.h"

#define INO_FLAG_CLEAR(flags)          { flags = ino::FLAG_NONE; }
#define INO_FLAG_SET(flags, mask)      { flags |=   (mask); }
#define INO_FLAG_UNSET(flags, mask)    { flags &= (~(mask)); }
#define INO_FLAG_GET(flags, mask)      ( (flags)&(mask) )

INO_NAMESPACE

enum { FLAG_NONE = 0x0 };
typedef ino_flags       flags;

INO_NAMESPACE_END

#endif    /*INO_FLAGS_H*/
