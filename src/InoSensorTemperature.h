#ifndef __INO_SENSOR_TEMPERATURE_H__
#define __INO_SENSOR_TEMPERATURE_H__
#include <DHT.h>
#include "InoSensorObj.h"

#ifdef ESP8266
#include <functional>
typedef std::function<void(const ino::handle caller, const float temperature, const float humidity)> OnUpdateTemperatureSignature;
#else
typedef void (*OnUpdateTemperatureSignature)(const ino::handle caller, const float temperature, const float humidity);
#endif

#define AM2320_TYPE                 (AM2320)
#define DHT22_TYPE                  (DHT22)    // DHT 22  (AM2302), AM2321

INO_NAMESPACE


typedef OnUpdateTemperatureSignature OnUpdateTemperatureCallback;

class SensorTemperature : public SensorObj {
public:
  SensorTemperature(
    const uint8_t pin,
    const uint8_t type,
    OnUpdateTemperatureCallback callback,
    const handle caller,
    const rtime_t interval=MY_INO_SENSOR_UPDATE_TEMPERATURE_INTERVAL,     // in seconds
    const bool celsius=true,
    const uint8_t rearm_count=8,
    const rtime_t rearm_interval=5);        // in seconds
  
  bool      loop(void);
  
private:
  DHT                         m_sensor;
  OnUpdateTemperatureCallback m_callback;
  const handle                m_caller;
  rtime_t                     m_rearm_interval;
  uint8_t                     m_rearm_count;
  uint8_t                     m_count;
};

INO_NAMESPACE_END

#endif    /*__INO_SENSOR_TEMPERATURE_H__*/
