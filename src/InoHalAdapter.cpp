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
