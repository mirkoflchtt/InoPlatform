#ifndef __INO_CRC_H
#define __INO_CRC_H
#include "InoTypes.h"

INO_NAMESPACE

ino_u16    crc16_CCITT(
  const ino_u8* data_start,
  const ino_u8* data_end);

ino_u16    fletcher16(
  const ino_u8* data_start,
  const ino_u8* data_end);
          
ino_u32    fletcher32(
  const ino_u8* data_start,
  const ino_u8* data_end);
          
INO_NAMESPACE_END

#endif    /*__INO_CRC_H*/
