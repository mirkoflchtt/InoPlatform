#ifndef __INO_DATE_TIME_H
#define __INO_DATE_TIME_H
#include "InoTypes.h"
#include "InoHalAdapter.h"

// i.e. SEVENTY_YEARS = 2208988800UL are seconds from 
// Jan 1 1900 to Jan 1 1970 that is the epoch
#define INO_SEVENTY_YEARS       (2208988800UL)
#define INO_NTP_PACKET_SIZE     (48)

#define DATETIME_FMT            "%llu"

INO_NAMESPACE

typedef ino_u64        datetime_ts;

String printDateTime(const datetime_ts ts);

class DateTime {
public:
  DateTime(void);

  datetime_ts    now_ms(void);
  ino_u32        epoch_ts(void);

  void set_base_ts(const datetime_ts base_ms, const ino_i8 timezone=0);
    
  datetime_ts ntp_to_datetime(const ino_u8* ntp_packet);

private:
  void          handle_overflow(void);

  datetime_ts   m_base_ms;
  clock_ts      m_last_ts;
  ino_u32       m_epoch_ts;
  ino_i8        m_timezone;
};

INO_NAMESPACE_END

#endif    /*__INO_DATE_TIME_H*/
