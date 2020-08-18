#ifndef INO_SENSOR_OBJ_H
#define INO_SENSOR_OBJ_H
#include "InoTypes.h"
#include "InoFlags.h"
#include "InoHalAdapter.h"
#include "InoEventHandler.h"

INO_NAMESPACE

class SensorObj {
public:
  SensorObj(
    EventHandler& event_handler,
    const ino_u8 sensor_id,
    const ino_u8 pin,
    const ino_u8 type,
    const ino_interval interval);
  
protected:
  ino_bool    trigger(const ino_bool auto_rearm=true);
  void        rearm(ino_interval interval=0);
  void        setInterval(const ino_interval interval);

  EventHandler&           m_event_handler;
  EventHandler::Listener  m_event_listener;
  const ino_u8    m_sensor_id;
  const ino_u8    m_pin;
  const ino_u8    m_type;
  ino_interval    m_interval;
  /* 8 MSB bits are reserved flags */
  ino_flags       m_state;
  ino_timestamp   m_last_ts;
};

INO_NAMESPACE_END

#endif    /*INO_SENSOR_OBJ_H*/
