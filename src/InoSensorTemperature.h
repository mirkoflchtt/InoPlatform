#ifndef INO_SENSOR_TEMPERATURE_H
#define INO_SENSOR_TEMPERATURE_H
#include <SmartDHT.h>
#include "InoEventHandler.h"
#include "InoSensorObj.h"

#define DHT22_TYPE                  (DHT22)    // DHT 22  (AM2302), AM2321

INO_NAMESPACE

class SensorTemperature : public SensorObj {
public:
  SensorTemperature(
    EventHandler& event_handler,
    const ino_u8 sensor_id,
    const ino_u8 pin,
    const ino_u8 type,
    const ino_interval interval=MY_INO_SENSOR_UPDATE_TEMPERATURE_INTERVAL,     // in seconds
    const ino_bool celsius=true,
    const ino_u8 rearm_count=8,
    const ino_interval rearm_interval=5);        // in seconds
  
  ino_bool      loop(void);
  
private:
  SmartDHT                    m_sensor;
  ino_interval                m_rearm_interval;
  ino_u8                      m_rearm_count;
  ino_u8                      m_count;
};

INO_NAMESPACE_END

#endif    /*INO_SENSOR_TEMPERATURE_H*/
