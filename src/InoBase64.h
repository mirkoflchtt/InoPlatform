#ifndef INO_BASE64_H
#define INO_BASE64_H
#include "InoTypes.h"

INO_NAMESPACE

INO_API_DECLARE

enum {
    BASE64_ERROR_NONE               =  0,
    BASE64_ERROR_BUFFER_SMALL       = -1,
    BASE64_ERROR_INVALID_CHARACTER  = -2,
};

ino_error encodeBase64(
  char* dst, ino_size* dst_size,
  const ino_u8* src, const ino_size src_size);

ino_error decodeBase64(
  ino_u8* dst, ino_size* dst_size,
  const char* src, const ino_size src_size);

INO_API_DECLARE_END

INO_NAMESPACE_END

#endif      /*INO_BASE64_H*/
