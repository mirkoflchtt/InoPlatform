#ifdef INO_LOG_HAS_DATETIME
#include <time.h>
#endif

#include "InoLog.h"


#ifndef INO_LOG_BUFFER_SIZE
#define INO_LOG_BUFFER_SIZE       (256)
#endif

INO_NAMESPACE

static const char *level_names[] = {
  "SUDO ", "FATAL", "ERROR", "WARN ", "INFO ", "DEBUG", "TRACE"
};

#ifdef INO_LOG_USE_COLOR
static const char *level_colors[] = {
  "\x1b[33m", "\x1b[35m", "\x1b[31m", "\x1b[33m", "\x1b[32m", "\x1b[36m", "\x1b[94m"
};
#endif

class Log {
public:
  Log( const char* name="INO LOG" ) :
    m_name( name ),
    m_quiet( false ),
    m_log_level(LOG_ERROR),
    m_stream( NULL ),
    m_mqtt( NULL ),
    m_mqtt_topic( NULL )
  {
  }
  
  void  setName(const char* name)
  {
    m_name = name;
  }

  void setQuiet(const bool quiet)
  {
    m_quiet = quiet;
  }

  void setLevel(const LogLevel level)
  {
    m_log_level = level;
  }

  void  setStream(Stream* stream);

  void  setMQTT(PubSubClient* mqtt, const char* mqtt_topic);
  
  uint32_t log(
    const LogLevel level,
    const char *file, const int line,
    const char* fmt, va_list ap);

  //void  print( const char* fmt, ... );
  //void  log(const char* fmt, ... );
  //void  debug( const char* fmt, ... );
  
private:
  const char*     m_name;
  bool            m_quiet;
  LogLevel        m_log_level;

  Stream*         m_stream;
  
  PubSubClient*   m_mqtt;
  const char*     m_mqtt_topic;
};

void  Log::setMQTT( PubSubClient* mqtt, const char* mqtt_topic )
{
  m_mqtt_topic  = (mqtt) ? mqtt_topic : NULL;
  m_mqtt        = (mqtt_topic) ? mqtt : NULL;
  if ( m_mqtt ) {
    printf("#### [Module %s] Enabled MQTT Logging ####\n", m_name);
  } else {
    printf("#### [Module %s] Disabled MQTT Logging ####\n", m_name);
  }
    
}   

void  Log::setStream( Stream* stream )
{
  m_stream    = stream;
  
  if ( m_stream ) {
    printf("#### [Module %s] Enabled Stream Logging ####\n", m_name);
  } else {
    printf("#### [Module %s] Disabled Stream Logging ####\n", m_name);
  }
} 



uint32_t  Log::log(
  const LogLevel level,
  const char *file, const int line,
  const char* fmt, va_list ap)
{
  char buf[INO_LOG_BUFFER_SIZE];

  if ( level>m_log_level ) {
    return 0;
  }

  if ( m_quiet || !(m_mqtt || m_stream) ) {
    return 0;
  }
    
#ifdef INO_LOG_STRIP_FILE_PATH
  if ( (file=strrchr(file, INO_LOG_STRIP_FILE_PATH)) ) {
    file++;
  }
#endif

  uint32_t written = 0;

#ifdef INO_LOG_HAS_DATETIME
  /* Get current time */
  time_t t = time(NULL);
  struct tm *lt = localtime(&t);

  written += strftime(buf, sizeof(buf), "[%H:%M:%S]", lt);
  buf[written] = '\0';
#endif

#ifdef INO_LOG_USE_COLOR
  written += snprintf(buf+written, sizeof(buf)-written,
    "[%s][%s%-5s\x1b[0m \x1b[90m] %s:%d:\x1b[0m ",
    m_name, level_colors[level], level_names[level], file, line);
#else
  written += snprintf(buf+written, sizeof(buf)-written,
    "[%s][%-5s] %s:%d: ",
    m_name, level_names[level], file, line);
#endif

  written += vsnprintf(buf+written, sizeof(buf)-written, fmt, ap);
  buf[INO_LOG_BUFFER_SIZE-1] = '\0';

  if ( m_mqtt && m_mqtt->connected() ) {
    m_mqtt->publish(m_mqtt_topic, buf);
  }

  if ( written<sizeof(buf)-3 ) {
    written += snprintf(buf+written, sizeof(buf)-written, INO_CR);
  }
  buf[INO_LOG_BUFFER_SIZE-1] = '\0';

  if ( m_stream ) {
    m_stream->print(buf);
  }

  //printf("### written(%d)\r\n", written);

  return written;
}


static Log g_InoLog("INO LOG v." INO_LOG_VERSION);


INO_API_DECLARE

void logSetName(
  const char* name)
{
  if ( !name ) return;
  g_InoLog.setName(name);
}

void logSetQuiet(
  const bool quiet)
{
  g_InoLog.setQuiet(quiet);
}

void logSetLevel(
  const LogLevel level)
{
  g_InoLog.setLevel(level);
}

void logMqttEnable(
  PubSubClient* mqtt, const char* mqtt_topic)
{
  if ( !(mqtt && mqtt_topic) ) return;
  g_InoLog.setMQTT(mqtt, mqtt_topic);
}

void logMqttDisable(void)
{
  g_InoLog.setMQTT(NULL, NULL);
}

void logStreamEnable(
  Stream* stream)
{
  if ( !stream ) return;
  g_InoLog.setStream(stream);
}

void logStreamDisable(void)
{
  g_InoLog.setStream(NULL);
}

uint32_t logMsg(
  const LogLevel level,
  const char *file, const int line,
  const char* fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  const uint32_t written = g_InoLog.log(level, file, line, fmt, ap);
  va_end(ap);
  return written;
}

INO_API_DECLARE_END


INO_NAMESPACE_END
