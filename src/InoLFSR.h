#ifndef __INO_LFSR_H
#define __INO_LFSR_H
#include "InoMyConfig.h"
#include "InoTypes.h"


INO_NAMESPACE

INO_API_DECLARE

void      generateLFSRSequence(
  ino_u16* data,
  const ino_size num,
  const ino_u16 seed);

void      testLFSR(
  const ino_u16 seed);

INO_API_DECLARE_END

INO_NAMESPACE_END

#endif   /*__INO_LFSR_H*/
