#include "InoHalAdapter.h"

INO_NAMESPACE

class Millis {
public:
  Millis(const clock_ts base_ts) :
  m_base_ts(base_ts-millis())
  {

  }

  clock_ts    now(void)
  {
    return m_base_ts + millis();
  }
     
private:
  clock_ts         m_base_ts;
};

class Micros {
public:
  Micros(const clock_ts base_ts) :
  m_base_ts(base_ts-micros())
  {

  }

  clock_ts    now(void)
  {
    return m_base_ts + micros();
  }
     
private:
  clock_ts         m_base_ts;
};

INO_API_DECLARE

ino_u8 trigger_event(
  const clock_ts ts,
  const clock_ts last_ts,
  const delay_ts interval_ts)
{
  const clock_ts next_ts = last_ts + interval_ts;
  const ino_u8 mask = (ts>=next_ts)|((next_ts<last_ts)<<1)|((ts<last_ts)<<2);
  const ino_u8 out  = (mask & 0xFE) ? ((mask==0x4)|(mask==0x7)) : mask;

  //printf("  ## mask(0x%02x) -> %d\n", mask, out);
  return out;
}

delay_ts elapsed_ms(
  const delay_ts ts,
  const delay_ts last_ts)
{
  // return (ts-last_ts);
  return (ts<last_ts) ? ((~0x0U)-last_ts)+ts : (ts-last_ts);
}

clock_ts clock_ms(void)
{
  static Millis s_clock(0);
  return s_clock.now();
}

clock_ts clock_us(void)
{
  static Micros s_clock(0);
  return s_clock.now();
}

void wait_ms(const delay_ts ts)
{
  delay(ts);
}

void wait_us(const delay_ts ts)
{
  delayMicroseconds(ts);
}

INO_API_DECLARE_END

INO_NAMESPACE_END
