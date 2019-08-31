#ifndef __INO_CRC_H__
#define __INO_CRC_H__
#include "InoTypes.h"

INO_NAMESPACE

uint16_t    crc16_CCITT(
  const uint8_t* data_start,
  const uint8_t* data_end);

uint16_t    fletcher16(
  const uint8_t* data_start,
  const uint8_t* data_end);
          
uint32_t    fletcher32(
  const uint8_t* data_start,
  const uint8_t* data_end);
          
INO_NAMESPACE_END

#endif    /*__INO_CRC_H__*/
