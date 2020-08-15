#include "InoHalAdapter.h"

INO_NAMESPACE

class Millis {
public:
  Millis(const ino_timestamp base_ts) :
  m_base_ts(base_ts-millis())
  {

  }

  ino_timestamp    now(void)
  {
    return m_base_ts + millis();
  }
     
private:
  ino_timestamp         m_base_ts;
};

class Micros {
public:
  Micros(const ino_timestamp base_ts) :
  m_base_ts(base_ts-micros())
  {

  }

  ino_timestamp    now(void)
  {
    return m_base_ts + micros();
  }
     
private:
  ino_timestamp         m_base_ts;
};

INO_API_DECLARE

#if 0
INO_API_ENTRY
ino_bool trigger_event(
  const ino_timestamp ts,
  const ino_timestamp last_ts,
  const ino_interval interval_ts)
{
  const ino_timestamp next_ts = last_ts + interval_ts;
  const ino_u8 mask = (ts>=next_ts)|((next_ts<last_ts)<<1)|((ts<last_ts)<<2);
  const ino_u8 out  = (mask & 0xFE) ? ((mask==0x4)|(mask==0x7)) : mask;

  //printf("  ## mask(0x%02x) -> %d\n", mask, out);
  return (out>0) ? true : false;
}

INO_API_ENTRY
ino_interval elapsed_ms(
  const ino_timestamp ts,
  const ino_timestamp last_ts)
{
  return (ts<last_ts) ? (((~0x0U)-last_ts)+ts) : (ts-last_ts);
}
#else
INO_API_ENTRY
ino_bool trigger_event(
  const ino_timestamp ts,
  const ino_timestamp last_ts,
  const ino_interval interval_ts)
{
  return ((ts-last_ts)>=interval_ts);
}

INO_API_ENTRY
ino_interval elapsed_ms(
  const ino_timestamp ts,
  const ino_timestamp last_ts)
{
  return (ts-last_ts);
}
#endif

INO_API_ENTRY
ino_timestamp clock_ms(void)
{
  static Millis s_clock(0);
  return s_clock.now();
}

INO_API_ENTRY
ino_timestamp clock_us(void)
{
  static Micros s_clock(0);
  return s_clock.now();
}

INO_API_ENTRY
void wait_ms(const ino_interval ts)
{
  delay(ts);
}

INO_API_ENTRY
void wait_us(const ino_interval ts)
{
  delayMicroseconds(ts);
}

INO_API_DECLARE_END

INO_NAMESPACE_END
