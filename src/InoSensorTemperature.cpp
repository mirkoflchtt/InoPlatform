#include "InoSensorTemperature.h"

#define CELSIUS_FLAG           (0x1U<<0)

INO_NAMESPACE

SensorTemperature::SensorTemperature(
  const ino_u8 pin,
  const ino_u8 type,
  OnUpdateTemperatureCallback callback,
  const ino_handle caller,
  const delay_ts interval,
  const ino_bool celsius,
  const ino_u8 rearm_count,
  const delay_ts rearm_interval) :
SensorObj(pin, type, interval*1000),
m_sensor(pin, type),
m_callback(callback),
m_caller(caller),
m_rearm_interval(rearm_interval*1000),
m_rearm_count(rearm_count),
m_count(rearm_count)
{
  m_state |= (celsius) ? CELSIUS_FLAG : 0x0;
}

bool SensorTemperature::loop(void)
{
  m_sensor.loop();

  if ( m_callback && trigger(true) ) {
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
      m_callback(m_caller, t, h);
      return true;
    } else {
      if (m_count>0) {
        printf("[Warning] SensorTemperature::loop() count(%u) rearm(%u)" INO_CR, 
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

