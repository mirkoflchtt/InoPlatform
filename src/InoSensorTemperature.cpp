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

    /*
     * Reading temperature or humidity takes about 250 milliseconds!
     * Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
     */

    /* Read humidity */    
    const ino_float h = m_sensor.readHumidity();

    /* Read temperature as Celsius or Farenheit */
    ino_float t     = m_sensor.readTemperature(isFahrenheit);
    
    if ( !(isnan(h)||isnan(t)) ) {
      m_count = m_rearm_count;

      /* Optional step: compute heat index considering humidity factor */
      t = m_sensor.computeHeatIndex(t, h, isFahrenheit);
      return m_event_handler.pushEventTemperatureHumidity(m_sensor_id, t, h);
    } else {
      if (m_count>0) {
        INO_LOG_INFO("[Warning] SensorTemperature::loop() count(%u) rearm(%u)", 
               m_count, m_rearm_interval);
        rearm(m_rearm_interval);
        m_count--;
      }
    }
  }
  return false;
}
  
INO_NAMESPACE_END

#undef CELSIUS_FLAG

