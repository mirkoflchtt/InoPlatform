#include "InoSensorObj.h"

#define STATE_ENABLE       (0x1U<<31)
#define STATE_TRIGGERED    (0x1U<<30)

INO_NAMESPACE

SensorObj::SensorObj(
  const ino_u8 pin,
  const ino_u8 type,
  const delay_ts interval) :
m_pin(pin),
m_type(type),
m_interval(interval),
m_state(0x0),
m_last_ts(clock_ms())
{
  m_state |= (interval>0) ? STATE_ENABLE : 0x0;
}

bool SensorObj::trigger(
  const ino_bool auto_rearm)
{
  if (0==(m_state & STATE_ENABLE)) {
    m_state &= (~STATE_TRIGGERED);
    return false;
  }

  const clock_ts ts = clock_ms();
  //if (ts < m_last_ts+m_interval) {
  if ( !trigger_event(ts, m_last_ts, m_interval) ) {
    m_state &= (~STATE_TRIGGERED);
    return false;
  }

  if (auto_rearm && (m_interval>0)) {
    rearm(m_interval);
  } else {
    m_state &= (~STATE_ENABLE);
  }

  m_state |= STATE_TRIGGERED;
  return true;
}

void SensorObj::rearm(
  delay_ts interval)
{
  interval  = (interval>0) ? interval : m_interval;
  m_last_ts = clock_ms();
  m_state   = (interval>0) ? (m_state | (STATE_ENABLE))
                           : (m_state & (~STATE_ENABLE));
}

void SensorObj::setInterval(
  const delay_ts interval)
{
  m_interval = interval;
  m_state   = (interval>0) ? (m_state | (STATE_ENABLE))
                           : (m_state & (~STATE_ENABLE));
}

INO_NAMESPACE_END

#undef STATE_ENABLE
#undef STATE_TRIGGERED
