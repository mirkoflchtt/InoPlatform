/**
 * Copyright (c) 2017 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See `log.c` for details.
 */

#ifndef __LOG_H_
#define __LOG_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

#define LOG_VERSION "0.2.0"


/******************************************************************************/
//#define LOG_USE_COLOR
//#define LOG_HAS_FILE_OUT
//#define LOG_HAS_DATETIME
#define LOG_STRIP_FILE_PATH     '/'
//#define LOG_STRIP_FILE_PATH     '\\'

/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

typedef void*   handle_t;

typedef void (*log_LockFn)(const handle_t udata, const bool lock);

typedef enum { 
  LOG_SUDO    = 0x0,
  LOG_FATAL,
  LOG_ERROR,
  LOG_WARN,
  LOG_INFO,
  LOG_DEBUG,
  LOG_TRACE,
} log_level_t;


#define log_sudo(...)  log_log(NULL, LOG_SUDO,  __FILE__, __LINE__, __VA_ARGS__)
#define log_trace(...) log_log(NULL, LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...) log_log(NULL, LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...)  log_log(NULL, LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...)  log_log(NULL, LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) log_log(NULL, LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) log_log(NULL, LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

void log_set_udata(const handle_t udata);
void log_set_lock(log_LockFn fn);

#ifdef LOG_HAS_FILE_OUT
void log_set_fp(FILE *fp);
#endif

void log_set_level(const log_level_t level);
void log_set_quiet(const bool enable);

bool log_set_buffer(char* buffer, const uint32_t buffer_size);

uint32_t log_log(
  const char* prefix,
  const log_level_t level,
  const char *file, const int line,
  const char *fmt, va_list args);

#ifdef __cplusplus
}
#endif

#endif    /*__LOG_H_*/
