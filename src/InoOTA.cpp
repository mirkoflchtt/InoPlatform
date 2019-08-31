#include "InoOTA.h"
#include "InoHalAdapter.h"

INO_NAMESPACE

static
void otaOnStart( void )
{
  printf("[OTA] Start..\n");
}

static
void otaOnEnd( void )
{
  printf("[OTA] End!\n");
}

static
void otaOnProgress( unsigned int progress, unsigned int total )
{
  printf("[OTA] Progress : (%u/%u)\n", progress, total);
}

static
void otaOnError( ota_error_t error )
{
  printf("Error[%u] : ", error);
  switch ( error ) {
    case OTA_AUTH_ERROR:    printf("  Authentication Failed\n"); break;
    case OTA_BEGIN_ERROR:   printf("  Begin Failed\n"); break;
    case OTA_CONNECT_ERROR: printf("  Connect Failed\n"); break;
    case OTA_RECEIVE_ERROR: printf("  Receive Failed\n"); break;
    case OTA_END_ERROR:     printf("  End Failed\n"); break;
    default: break;
  }
  ESP.restart();
}

OTA::OTA(
  const char* host,
  const char* password,
  const uint16_t port,
  OTA_CALLBACK(on_start),
  OTA_CALLBACK(on_end),
  OTA_CALLBACK_PROGRESS(on_progress),
  OTA_CALLBACK_ERROR(on_error) ) :
  m_initiliazed( false ),
  m_host( host ),
  m_password( password ),
  m_port( port ),
  m_on_start( (on_start) ? on_start : otaOnStart ),
  m_on_end( (on_end) ? on_end : otaOnEnd ),
  m_on_progress( (on_progress) ? on_progress : otaOnProgress ),
  m_on_error( (on_error) ? on_error : otaOnError )
{
}

OTA::~OTA( void )
{
}

bool OTA::connected( void )
{
  return ( WiFi.status()==WL_CONNECTED );
}

bool OTA::init( const char* wifi_ssid, const char* wifi_password, const bool wait )
{
  if ( !wifi_ssid )
    return false;

  WiFi.mode( WIFI_STA );
  WiFi.begin( wifi_ssid, wifi_password );

  while ( wait && (WiFi.status() != WL_CONNECTED) ) {
    printf(".");
    ino::wait_ms(500);
  }
  printf(" \n");
  m_initiliazed = false;
  return true;
}


bool OTA::loop( void )
{
  if ( WiFi.status() == WL_CONNECTED )
  {
    if ( !m_initiliazed ) {
      // This is the 1st time a connection is established.. be more verbose for just this time..
//      Serial.printf( "[OTA::loop]            : WiFI Connected to SSID \"%s\"on channel %u\n", INO_TO_CSTRING(WiFi.SSID()), WiFi.channel() );
//      Serial.print( "[OTA::loop] IP address  : "); Serial.println( WiFi.localIP() );

      if ( m_host )
        ArduinoOTA.setHostname(m_host);
      if ( m_password )
        ArduinoOTA.setPassword(m_password);

      ArduinoOTA.setPort(m_port);
    
      ArduinoOTA.onStart(m_on_start);
      ArduinoOTA.onEnd(m_on_end);
      ArduinoOTA.onProgress(m_on_progress);
      ArduinoOTA.onError(m_on_error);

      ArduinoOTA.begin();
      m_initiliazed = true;

      printf("OTA : Active on port : %u\n", m_port);
    }

    if ( m_initiliazed ) {
      //printf("OTA : looping..\n");
      ArduinoOTA.handle();
    }
    return true;
  }

  m_initiliazed = false;
  return false;
}

INO_NAMESPACE_END
