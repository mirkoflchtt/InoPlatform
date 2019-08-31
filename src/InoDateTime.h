#ifndef __INO_DATE_TIME_H__
#define __INO_DATE_TIME_H__
#include "InoTypes.h"
#include "InoHalAdapter.h"

// i.e. SEVENTY_YEARS = 2208988800UL are seconds from 
// Jan 1 1900 to Jan 1 1970 that is the epoch
#define INO_SEVENTY_YEARS       (2208988800UL)
#define INO_NTP_PACKET_SIZE     (48)

#define DATETIME_FMT            "%llu"

INO_NAMESPACE

typedef uint64_t        datetime_ts;

String printDateTime(const ino::datetime_ts ts);

class DateTime {
public:
  DateTime(void);

  datetime_ts    now_ms(void);
  uint32_t       epoch_ts(void);

  void set_base_ts(const datetime_ts base_ms, const int8_t timezone=0);
    
  datetime_ts ntp_to_datetime(const uint8_t* ntp_packet);

private:
  void          handle_overflow(void);

  datetime_ts   m_base_ms;
  clock_ts      m_last_ts;
  uint32_t      m_epoch_ts;
  int8_t        m_timezone;
};

INO_NAMESPACE_END

#endif    /*__INO_DATE_TIME_H__*/
