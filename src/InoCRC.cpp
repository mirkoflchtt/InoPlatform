#include "InoCRC.h"

INO_NAMESPACE


uint16_t    crc16_CCITT(
  const uint8_t* data_start,
  const uint8_t* data_end)
{
  uint16_t crc = 0xFFFF;
  uint8_t x;
      
  while ( data_start<data_end )
  {
    x  = crc >> 8 ^ *data_start++;
    x ^= x>>4;
    crc = (crc<<8) ^ ((uint16_t)(x<<12)) ^ ((uint16_t)(x<<5)) ^ ((uint16_t)x);
  }
  return crc;
}


uint16_t    fletcher16(
  const uint8_t* data_start,
  const uint8_t* data_end)
{
  uint16_t sum1 = 0;
  uint16_t sum2 = 0;

  for( ; data_start<data_end; data_start++ )
  {
    sum1 = (sum1 + *data_start) & 0xFF;
    sum2 = (sum1 + sum2) & 0xFF;
  }

  return (sum2 << 8) | sum1;  
}           

          
uint32_t    fletcher32(
  const uint8_t* data_start,
  const uint8_t* data_end)
{
#define SUM_REDUCTION(sum)    ( ((sum) & 0xffff) + ((sum) >> 16) )
  uint32_t words      = (data_end-data_start);
  const uint16_t* data;
  uint32_t sum1, sum2;
  
  sum1 = sum2 = 0xffff - data_start[0];
  
  if ( words & 0x1 )
    data_start++;
    
  words >>= 1;
 
  data = (const uint16_t*)data_start;
  
  while ( words )
  {
    uint32_t tlen = ((words >= 359) ? 359 : words);
    words -= tlen;
    do {
      sum2 += sum1 += *data++;
      tlen--;
    } while ( tlen );
    sum1 = SUM_REDUCTION(sum1);
    sum2 = SUM_REDUCTION(sum2);
  }
  
  // Second reduction step to reduce sums to 16 bits
  sum1 = SUM_REDUCTION(sum1);
  sum2 = SUM_REDUCTION(sum2);

  return ( (sum2 << 16) | sum1 );
#undef SUM_REDUCTION
}

  
INO_NAMESPACE_END
