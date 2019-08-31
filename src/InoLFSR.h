#ifndef __INO_LFSR_H__
#define __INO_LFSR_H__
#include "InoMyConfig.h"
#include "InoTypes.h"


INO_NAMESPACE

INO_API_DECLARE

void      generateLFSRSequence(
  uint16_t* data,
  const size_t num,
  const uint16_t seed);

void      testLFSR(
  const uint16_t seed);

INO_API_DECLARE_END

INO_NAMESPACE_END

#endif   /*__INO_LFSR_H__*/
