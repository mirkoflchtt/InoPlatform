#ifndef __INO_SENSOR_OBJ_H
#define __INO_SENSOR_OBJ_H
#include "InoTypes.h"
#include "InoHalAdapter.h"

INO_NAMESPACE

class SensorObj {
public:
  SensorObj(
    const ino_u8 pin,
    const ino_u8 type,
    const delay_ts interval);
  
protected:
  ino_bool    trigger(const ino_bool auto_rearm=true);
  void        rearm(delay_ts interval=0);
  void        setInterval(const delay_ts interval);

  const ino_u8    m_pin;
  const ino_u8    m_type;
  delay_ts        m_interval;
  /* 8 MSB bits are reserved flags */
  ino_u32         m_state;
  clock_ts        m_last_ts;
};

INO_NAMESPACE_END

#endif    /*__INO_SENSOR_OBJ_H*/
