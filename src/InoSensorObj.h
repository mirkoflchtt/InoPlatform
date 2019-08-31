#ifndef __INO_SENSOR_OBJ_H__
#define __INO_SENSOR_OBJ_H__
#include "InoTypes.h"
#include "InoHalAdapter.h"

INO_NAMESPACE

class SensorObj {
public:
  SensorObj(
    const uint8_t pin,
    const uint8_t type,
    const rtime_t interval);
  
protected:
  bool        trigger(const bool auto_rearm=true);
  void        rearm(rtime_t interval=0);
  void        setInterval(const rtime_t interval);

  const uint8_t   m_pin;
  const uint8_t   m_type;
  rtime_t         m_interval;
  /* 8 MSB bits are reserved flags */
  uint32_t        m_state;
  clock_ts        m_next_ts;
};

INO_NAMESPACE_END

#endif    /*__INO_SENSOR_OBJ_H__*/
