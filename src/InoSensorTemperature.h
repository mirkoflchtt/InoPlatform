#ifndef __INO_SENSOR_TEMPERATURE_H
#define __INO_SENSOR_TEMPERATURE_H
#include <DHT.h>
#include "InoSensorObj.h"

#ifdef ESP8266
#include <functional>
typedef std::function<void(const ino_handle caller, const ino_float temperature, const ino_float humidity)> OnUpdateTemperatureSignature;
#else
typedef void (*OnUpdateTemperatureSignature)(const ino_handle caller, const ino_float temperature, const ino_float humidity);
#endif

#define AM2320_TYPE                 (AM2320)
#define DHT22_TYPE                  (DHT22)    // DHT 22  (AM2302), AM2321

INO_NAMESPACE


typedef OnUpdateTemperatureSignature OnUpdateTemperatureCallback;

class SensorTemperature : public SensorObj {
public:
  SensorTemperature(
    const ino_u8 pin,
    const ino_u8 type,
    OnUpdateTemperatureCallback callback,
    const ino_handle caller,
    const delay_ts interval=MY_INO_SENSOR_UPDATE_TEMPERATURE_INTERVAL,     // in seconds
    const ino_bool celsius=true,
    const ino_u8 rearm_count=8,
    const delay_ts rearm_interval=5);        // in seconds
  
  ino_bool      loop(void);
  
private:
  DHT                         m_sensor;
  OnUpdateTemperatureCallback m_callback;
  const ino_handle            m_caller;
  delay_ts                    m_rearm_interval;
  ino_u8                      m_rearm_count;
  ino_u8                      m_count;
};

INO_NAMESPACE_END

#endif    /*__INO_SENSOR_TEMPERATURE_H*/
