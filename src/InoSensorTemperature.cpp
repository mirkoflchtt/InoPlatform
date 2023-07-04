#include "InoSensorTemperature.h"
#include "InoLog.h"

#define CELSIUS_FLAG           (0x1U<<0)

INO_NAMESPACE

SensorTemperature::SensorTemperature(
  EventHandler& event_handler,
  const ino_u8 sensor_id,
  const ino_u8 pin,
  const ino_u8 type,
  const ino_interval interval,
  const ino_bool celsius,
  const ino_u8 rearm_count,
  const ino_interval rearm_interval) :
SensorObj(event_handler, sensor_id, pin, type, interval*1000U),
m_sensor(pin, type),
m_rearm_interval(rearm_interval*1000U),
m_rearm_count(rearm_count),
m_count(rearm_count)
{
  m_state |= (celsius) ? CELSIUS_FLAG : 0x0;
}

ino_bool SensorTemperature::loop(void)
{
  m_sensor.loop();

  if (trigger(true)) {
    const ino_bool isFahrenheit = (m_state & CELSIUS_FLAG) ? false : true;
    ino_float t = NAN, h = NAN;

    /*
     * Reading temperature or humidity takes about 250 milliseconds!
     * Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
     */
    if (m_sensor.read() == SMARTDHT_OK)
    {
      /* Read humidity */   
      h = m_sensor.readHumidity();

      /* Read temperature as Celsius or Farenheit */
      t = m_sensor.readTemperature(isFahrenheit);
    }

    if ( isnan(h) || isnan(t) ) {
      if (m_count>0) {
        INO_LOG_INFO("[Warning] SensorTemperature::loop() count(%u) rearm(%u)", m_count, m_rearm_interval);
        rearm(m_rearm_interval);
        m_count--;
      }
    } else {
      ino_bool ok = true;
      m_count = m_rearm_count;

      /* Optional step: compute heat index considering humidity factor */
      // t = m_sensor.computeHeatIndex(t, h, isFahrenheit);
      ok &= m_event_handler.pushEventTemperature(m_sensor_id, t);
      ok &= m_event_handler.pushEventHumidity(m_sensor_id, h);
      return ok;
    }
  }
  return false;
}
  
INO_NAMESPACE_END

#undef CELSIUS_FLAG

