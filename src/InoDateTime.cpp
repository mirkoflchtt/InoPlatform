#include <time.h>
#include "InoDateTime.h"
#include "InoLog.h"


#define MSEC_IN_A_HOUR      (60*60*1000)

INO_NAMESPACE

String printDateTime(const datetime_ts ts)
{
  const time_t now   = ts / 1000;
  char* datetime     = ctime(&now);
  datetime[strlen(datetime)-1] = '\0';
  
  return String(datetime);
}

DateTime::DateTime(void) :
m_base_ms(0),
m_last_ts(clock_ms()),
m_epoch_ts(0),
m_timezone(0)
{
}

datetime_ts DateTime::now_ms(void)
{
  handle_overflow();

  return m_base_ms + m_last_ts;
}

ino_u32 DateTime::epoch_ts(void)
{
  return m_epoch_ts;
}

void DateTime::handle_overflow(void)
{
  const ino_timestamp ts = clock_ms();
  if ( ts<m_last_ts ) {
    m_base_ms += 1;
    m_base_ms += CLOCK_MAX_VALUE;
    INO_LOG_DEBUG("DateTime::handle_overflow detected: " DATETIME_FMT, m_base_ms);
  }
  m_last_ts = ts;
}

void DateTime::set_base_ts(
  const datetime_ts base_ms, const ino_i8 timezone)
{
  const ino_timestamp ts = clock_ms();
  const datetime_ts offset = (datetime_ts)timezone * MSEC_IN_A_HOUR - ts;
  
  m_timezone = timezone;
  m_last_ts  = ts;
  m_base_ms  = base_ms + offset;
}

datetime_ts DateTime::ntp_to_datetime(
  const ino_u8* ntp_packet)
{
  ino_u64 acc = 0;
  datetime_ts ts;
  ino_i32 i;

  // get seconds from 1900 from ntp packet (bytes [40-43])
  for ( i=INO_NTP_PACKET_SIZE-8; i<INO_NTP_PACKET_SIZE-4; i++ ) {
    acc <<= 8;
    acc |= ntp_packet[i];
  }
  // convert to epoch time (from 1st Jan 1970)
  acc -= INO_SEVENTY_YEARS;

  // convert seconds o milliseconds
  ts = (acc*1000);

  m_epoch_ts = (ino_u32)(acc);

  // get useconds part from ntp packet (bytes [44-47])
  acc = 0;
  for ( ; i<INO_NTP_PACKET_SIZE; i++ ) {
    acc <<= 8;
    acc |= ntp_packet[i];
  }
  acc *= 1000;
  acc >>= 32;

  // convert useconds o milliseconds
  ts += (acc);
  
  INO_LOG_DEBUG("DateTime::ntp_to_datetime : epoch ts(%u)", m_epoch_ts)

  return ts;
}

INO_NAMESPACE_END
