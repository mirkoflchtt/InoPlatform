#ifndef INO_OTA_H
#define INO_OTA_H
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "InoTypes.h"

INO_NAMESPACE

#ifndef OTA_CALLBACK
#define OTA_CALLBACK(func)            ArduinoOTAClass::THandlerFunction func
#endif

#ifndef OTA_CALLBACK_ERROR
#define OTA_CALLBACK_ERROR(func)      ArduinoOTAClass::THandlerFunction_Error func 
#endif

#ifndef OTA_CALLBACK_PROGRESS
#define OTA_CALLBACK_PROGRESS(func)   ArduinoOTAClass::THandlerFunction_Progress func  
#endif

class OTA {
public:
   OTA(
    const char* host=NULL,
    const char* password=NULL,
    const ino_u16 port=8266,
    OTA_CALLBACK(on_start)=NULL,
    OTA_CALLBACK(on_end)=NULL,
    OTA_CALLBACK_PROGRESS(on_progress)=NULL,
    OTA_CALLBACK_ERROR(on_error)=NULL );
    
  ~OTA(void);

  ino_bool    connected(void);
  
  ino_bool    init(const char* wifi_ssid, const char* wifi_password,
                   const ino_bool wait=false);
  ino_bool    loop(void);

private:
  ino_bool            m_initialiazed;
  const char*         m_host;
  const char*         m_password;
  const ino_u16       m_port;
  
  OTA_CALLBACK(m_on_start);
  OTA_CALLBACK(m_on_end);
  OTA_CALLBACK_PROGRESS(m_on_progress);
  OTA_CALLBACK_ERROR(m_on_error);
};

INO_NAMESPACE_END

#endif    /*INO_OTA_H*/
