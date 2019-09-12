#include "InoOTA.h"
#include "InoHalAdapter.h"
#include "InoLog.h"

INO_NAMESPACE

static
void otaOnStart(void)
{
  INO_LOG_INFO("[OTA] Start..")
}

static
void otaOnEnd(void)
{
  INO_LOG_INFO("[OTA] End!")
}

static
void otaOnProgress(unsigned int progress, unsigned int total)
{
  INO_LOG_INFO("[OTA] Progress : (%u/%u)", progress, total)
}

static
void otaOnError(ota_error_t error)
{
  switch ( error ) {
    case OTA_AUTH_ERROR:
      INO_LOG_ERROR("Error[%u] :   %s", error, "Authentication Failed")
      break;
    case OTA_BEGIN_ERROR:
      INO_LOG_ERROR("Error[%u] :   %s", error, "Begin Failed")
      break;
    case OTA_CONNECT_ERROR:
      INO_LOG_ERROR("Error[%u] :   %s", error, "Connect Failed")
      break;
    case OTA_RECEIVE_ERROR:
      INO_LOG_ERROR("Error[%u] :   %s", error, "Receive Failed")
      break;
    case OTA_END_ERROR:
      INO_LOG_ERROR("Error[%u] :   %s", error, "End Failed")
      break;
    default:
      INO_LOG_ERROR("Error[%u] : ", error)
      break;
  }
  ESP.restart();
}

OTA::OTA(
  const char* host,
  const char* password,
  const ino_u16 port,
  OTA_CALLBACK(on_start),
  OTA_CALLBACK(on_end),
  OTA_CALLBACK_PROGRESS(on_progress),
  OTA_CALLBACK_ERROR(on_error)) :
  m_initialiazed(false),
  m_host(host),
  m_password(password),
  m_port(port),
  m_on_start((on_start) ? on_start : otaOnStart),
  m_on_end((on_end) ? on_end : otaOnEnd),
  m_on_progress((on_progress) ? on_progress : otaOnProgress),
  m_on_error((on_error) ? on_error : otaOnError)
{
}

OTA::~OTA(void)
{
}

ino_bool OTA::connected(void)
{
  return (WiFi.status()==WL_CONNECTED);
}

ino_bool OTA::init(
  const char* wifi_ssid, const char* wifi_password, const ino_bool wait)
{
  if (!wifi_ssid) {
    return false;
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_password);

  while (wait && (WiFi.status() != WL_CONNECTED)) {
    printf(".");
    ino::wait_ms(500);
  }
  printf(" \n");
  m_initialiazed = false;
  return true;
}

ino_bool OTA::loop(void)
{
  if ( WiFi.status() == WL_CONNECTED ) {
    if (!m_initialiazed) {
      // This is the 1st time a connection is established.. be more verbose for just this time..
//      Serial.printf( "[OTA::loop]            : WiFI Connected to SSID \"%s\"on channel %u\n", INO_TO_CSTRING(WiFi.SSID()), WiFi.channel() );
//      Serial.print( "[OTA::loop] IP address  : "); Serial.println( WiFi.localIP() );

      if (m_host) {
        ArduinoOTA.setHostname(m_host);
      }
      if (m_password) {
        ArduinoOTA.setPassword(m_password);
      }
      ArduinoOTA.setPort(m_port);
    
      ArduinoOTA.onStart(m_on_start);
      ArduinoOTA.onEnd(m_on_end);
      ArduinoOTA.onProgress(m_on_progress);
      ArduinoOTA.onError(m_on_error);

      ArduinoOTA.begin();
      m_initialiazed = true;

      INO_LOG_INFO("OTA : Active on port : %u", m_port)
    }

    if (m_initialiazed) {
      //printf("OTA : looping..\n");
      ArduinoOTA.handle();
    }
    return true;
  }

  m_initialiazed = false;
  return false;
}

INO_NAMESPACE_END
