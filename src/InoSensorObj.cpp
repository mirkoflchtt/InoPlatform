#include "InoSensorObj.h"

#define STATE_ENABLE       (0x1U<<31)
#define STATE_TRIGGERED    (0x1U<<30)

INO_NAMESPACE

SensorObj::SensorObj(
  EventHandler& event_handler,
  const ino_u8 sensor_id,
  const ino_u8 pin,
  const ino_u8 type,
  const ino_interval interval) :
m_event_handler(event_handler),
m_event_listener(),
m_sensor_id(sensor_id),
m_pin(pin),
m_type(type),
m_interval(interval),
m_state(0x0),
m_last_ts(clock_ms() - interval)
{
  m_state |= (interval>0) ? STATE_ENABLE : 0x0;
}

ino_bool SensorObj::trigger(
  const ino_bool auto_rearm)
{
  if (0==(m_state & STATE_ENABLE)) {
    m_state &= (~STATE_TRIGGERED);
    return false;
  }

  const ino_timestamp ts = clock_ms();
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
  ino_interval interval)
{
  interval  = (interval>0) ? interval : m_interval;
  m_last_ts = clock_ms();
  m_state   = (interval>0) ? (m_state | (STATE_ENABLE))
                           : (m_state & (~STATE_ENABLE));
}

void SensorObj::setInterval(
  const ino_interval interval)
{
  m_interval = interval;
  m_state   = (interval>0) ? (m_state | (STATE_ENABLE))
                           : (m_state & (~STATE_ENABLE));
}

INO_NAMESPACE_END

#undef STATE_ENABLE
#undef STATE_TRIGGERED
