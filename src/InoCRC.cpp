#include "InoCRC.h"

INO_NAMESPACE


/*****************************************************************************/
ino_u16    crc16(
  ino_u16 crc,
  const ino_u8* data_start,
  const ino_u8* data_end)
{
  ino_u8 x;

  crc = ~crc;
  for ( ; data_start<data_end; ) {
    x = crc >> 8 ^ *data_start++;
    x ^= (x >> 4);
    crc = (crc<<8) ^ ((ino_u16)(x<<12)) ^ ((ino_u16)(x<<5)) ^ ((ino_u16)x);
  }
  return ~crc;
}

ino_u16    crc16_CCITT(
  const ino_u8* data_start,
  const ino_u8* data_end)
{
  return ~crc16(0x0, data_start, data_end);
}

/*****************************************************************************/
ino_u32    crc32(
  ino_u32 crc,
  const ino_u8* data_start,
  const ino_u8* data_end)
{
  crc = ~crc;
  for ( ; data_start<data_end; ) {
    crc ^= *data_start++;
    
    {
      crc = (crc & 1) ? ((crc >> 1) ^ 0xedb88320) : (crc >> 1);
      crc = (crc & 1) ? ((crc >> 1) ^ 0xedb88320) : (crc >> 1);
      crc = (crc & 1) ? ((crc >> 1) ^ 0xedb88320) : (crc >> 1);
      crc = (crc & 1) ? ((crc >> 1) ^ 0xedb88320) : (crc >> 1);

      crc = (crc & 1) ? ((crc >> 1) ^ 0xedb88320) : (crc >> 1);
      crc = (crc & 1) ? ((crc >> 1) ^ 0xedb88320) : (crc >> 1);
      crc = (crc & 1) ? ((crc >> 1) ^ 0xedb88320) : (crc >> 1);
      crc = (crc & 1) ? ((crc >> 1) ^ 0xedb88320) : (crc >> 1);
    }
  }
  return ~crc;
}

/*****************************************************************************/
ino_u16    fletcher16(
  const ino_u8* data_start,
  const ino_u8* data_end)
{
  ino_u16 sum1 = 0;
  ino_u16 sum2 = 0;

  for( ; data_start<data_end; data_start++ )
  {
    sum1 = (sum1 + *data_start) & 0xFF;
    sum2 = (sum1 + sum2) & 0xFF;
  }

  return (sum2 << 8) | sum1;  
}           

          
ino_u32    fletcher32(
  const ino_u8* data_start,
  const ino_u8* data_end)
{
#define SUM_REDUCTION(sum)    ( ((sum) & 0xffff) + ((sum) >> 16) )
  ino_u32 words      = (data_end-data_start);
  const ino_u16* data;
  ino_u32 sum1, sum2;
  
  sum1 = sum2 = 0xffff - data_start[0];
  
  if ( words & 0x1 )
    data_start++;
    
  words >>= 1;
 
  data = (const ino_u16*)data_start;
  
  while (words)
  {
    ino_u32 tlen = ((words >= 359) ? 359 : words);
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
