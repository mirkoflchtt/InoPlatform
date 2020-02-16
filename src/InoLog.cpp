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
    m_name(name),
    m_quiet(false),
    m_log_level(LOG_ERROR),
    m_stream(NULL),
    m_mqtt(NULL),
    m_mqtt_topic(NULL)
  {
  }
  
  void  setName(const char* name)
  {
    m_name = name;
  }

  void setQuiet(const ino_bool quiet)
  {
    m_quiet = quiet;
  }

  void setLevel(const LogLevel level)
  {
    m_log_level = level;
  }

  void  setStream(Stream* stream);

  void  setMQTT(PubSubClient* mqtt, const char* mqtt_topic);
  
  ino_u32 log(
    const LogLevel level,
    const char *file, const int line,
    const char* fmt, va_list ap);

  //void  print( const char* fmt, ... );
  //void  log(const char* fmt, ... );
  //void  debug( const char* fmt, ... );
  
private:
  const char*     m_name;
  ino_bool        m_quiet;
  LogLevel        m_log_level;

  Stream*         m_stream;
  
  PubSubClient*   m_mqtt;
  const char*     m_mqtt_topic;
};

void  Log::setMQTT(PubSubClient* mqtt, const char* mqtt_topic)
{
  m_mqtt        = (mqtt && mqtt_topic) ? mqtt : NULL;
  m_mqtt_topic  = (m_mqtt) ? mqtt_topic : NULL;
  
  if (m_mqtt) {
    INO_LOG_INFO("#### [Module %s] Enabled MQTT Logging: topic(%s) ####", m_name, m_mqtt_topic);
  } else {
    INO_LOG_INFO("#### [Module %s] Disabled MQTT Logging ####", m_name);
  }
}   

void  Log::setStream(Stream* stream)
{
  m_stream    = stream;
  
  if (m_stream) {
    INO_LOG_INFO("#### [Module %s] Enabled Stream Logging ####", m_name);
  } else {
    INO_LOG_INFO("#### [Module %s] Disabled Stream Logging ####", m_name);
  }
} 



ino_u32  Log::log(
  const LogLevel level,
  const char *file, const int line,
  const char* fmt, va_list ap)
{
  char buf[INO_LOG_BUFFER_SIZE];
  buf[0] = 0;

  if (level>m_log_level) {
    return 0;
  }

  if (m_quiet || !(m_mqtt || m_stream)) {
    return 0;
  }
    
#ifdef INO_LOG_STRIP_FILE_PATH
  if (file && (file=strrchr(file, INO_LOG_STRIP_FILE_PATH))) {
    file++;
  }
#endif

  ino_u32 written = 0;

#ifdef INO_LOG_HAS_DATETIME
  /* Get current time */
  time_t t = time(NULL);
  struct tm *lt = localtime(&t);

  written += strftime(buf, sizeof(buf), "[%H:%M:%S]", lt);
  buf[written] = 0;
#endif

#ifdef INO_LOG_USE_COLOR
  written += snprintf(buf+written, sizeof(buf)-written,
    "[%s][%s%-5s\x1b[0m \x1b[90m] %s:%d:\x1b[0m ",
    m_name, level_colors[level], level_names[level],
    ((file) ? file : ""), line);
#else
  written += snprintf(buf+written, sizeof(buf)-written,
    "[%s][%-5s] %s:%d: ",
    m_name, level_names[level],
    ((file) ? file : ""), line);
#endif

  written += vsnprintf(buf+written, sizeof(buf)-written, fmt, ap);
  buf[written] = 0;

  if (written>0) {
    if (m_mqtt && m_mqtt->connected()) {
      m_mqtt->publish(m_mqtt_topic, buf);
    }

    if (m_stream) {
      m_stream->printf("%s" INO_CR, buf);
    }

    //printf("### written(%u)\r\n", written);
  }

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
  const ino_bool quiet)
{
  g_InoLog.setQuiet(quiet);
}

void logSetLevel(
  const LogLevel level)
{
  g_InoLog.setLevel(level);
}

void logSetMqtt(
  PubSubClient* mqtt, const char* mqtt_topic)
{
  g_InoLog.setMQTT(mqtt, mqtt_topic);
}

void logSetStream(
  Stream* stream)
{
  g_InoLog.setStream(stream);
}

ino_u32 logMsg(
  const LogLevel level,
  const char *file, const int line,
  const char* fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  const ino_u32 written = g_InoLog.log(level, file, line, fmt, ap);
  va_end(ap);
  return written;
}

INO_API_DECLARE_END


INO_NAMESPACE_END
