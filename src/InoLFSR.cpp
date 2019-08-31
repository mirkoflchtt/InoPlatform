#include "InoLFSR.h"

INO_NAMESPACE

INO_API_DECLARE

// For implementation details looks at:
// https://en.wikipedia.org/wiki/Linear_feedback_shift_register
INO_DECLARE_STATIC
uint16_t fibonacciLFSR( const uint16_t lfsr )
{
  /* taps: 16 14 13 11; feedback polynomial: x^16 + x^14 + x^13 + x^11 + 1 */
  const uint16_t lsb  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 0x1;
  return ((lfsr >> 1) | (lsb << 15)); 
}

INO_DECLARE_STATIC
uint16_t galoisLFSR( const uint16_t lfsr )
{
  const uint16_t lsb = lfsr & 0x1;          /* Get LSB (i.e., the output bit). */
  return ((lfsr >> 1) ^ ((-lsb) & 0xB400)); /* If the output bit is 1, apply toggle mask.
                                             * The value has 1 at bits corresponding
                                             * to taps, 0 elsewhere. */
}

void generateLFSRSequence(
  uint16_t* data,
  const size_t num,
  const uint16_t seed )
{
  uint16_t* curr = data;
  uint16_t i;
  
  curr[0] = seed;
  for ( i=1; i<num; i++ )
  {
    curr[i] = galoisLFSR( curr[i-1] );
  }
  curr[0] = galoisLFSR( curr[i-1] );
}

void testLFSR( const uint16_t seed )
{
  Serial.println( "TestLFSR.." );
  const uint16_t start_state  = seed;  /* Any nonzero start state will work. */
  uint16_t lfsr               = start_state;
  uint32_t period             = 0;

  do {
    Serial.print( " Value : " );
    Serial.print( lfsr );
    Serial.print( " (Period : " );
    Serial.print( period );
    Serial.println( " )");
    //lfsr = fibonacciLFSR( lfsr );
    lfsr = galoisLFSR( lfsr );
    
    period++;
  } while ( lfsr!=start_state );
}

INO_API_DECLARE_END

INO_NAMESPACE_END
