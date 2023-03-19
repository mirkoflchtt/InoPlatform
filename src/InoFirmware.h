#ifndef INO_FIRMWARE_H
#define INO_FIRMWARE_H
#include "InoTypes.h"

#define INO_FIRMWARE_SIGNATURE(yyyy, mm, dd, hh, min, minor) \
  (ino::firmware_id)( ((((yyyy)-2000)&0xFF)<<24) | \
  (((mm)&0xF)<<20)  | \
  (((dd)&0x1F)<<15) | \
  (((hh)&0x1F)<<10) | \
  (((min)&0x3F)<<4) | \
  ((minor)&0xF) )

#define INO_FIRMWARE_GET(fw, yyyy, mm, dd, hh, min, minor) { \
  yyyy = (((fw)>>24)+2000); \
  mm   = (((fw)>>20)&0xF); \
  dd   = (((fw)>>15)&0x1F); \
  hh   = (((fw)>>10)&0x1F); \
  min  = (((fw)>>4)&0x3F); \
  minor= ((fw)&0xF); }    

INO_NAMESPACE

typedef ino_u32       firmware_id;


INO_DECLARE_STATIC
String getFirmwareVersion(const firmware_id fw)
{
  String firmware;
  ino_u16 year=0, month=0, day=0, hour=0, min=0, minor=0; 
  
  INO_FIRMWARE_GET(fw, year, month, day, hour, min, minor)
  //firmware.concat(String("Platform-"));
  firmware.concat(String(INO_PLATFORM_MAJOR));  firmware.concat(F("."));
  firmware.concat(String(INO_PLATFORM_MINOR));  firmware.concat(F("."));
  firmware.concat(String(INO_PLATFORM_MICRO));
  firmware.concat(String(INO_PLATFORM_EXTRA));  firmware.concat(F("-"));
  firmware.concat(String(year));  firmware.concat(F("-"));
  firmware.concat(String(month)); firmware.concat(F("-"));
  firmware.concat(String(day));   firmware.concat(F("-"));
  firmware.concat(String(hour));  firmware.concat(F("."));
  firmware.concat(String(min));   firmware.concat(F("."));
  firmware.concat(String(minor));
  
  return firmware;
}

INO_NAMESPACE_END

#endif    /*INO_FIRMWARE_H*/
