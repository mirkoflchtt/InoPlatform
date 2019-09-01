/*
 * Copyright (c) 2017 rxi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <stdlib.h>
#include <string.h>
#ifdef LOG_HAS_DATETIME
#include <time.h>
#endif

#include "log.h"

static
void dummy_lock(const handle_t udata, const bool lock)
{
  (void)(udata);
  (void)(lock);
}


static struct {
  handle_t      udata;
  log_LockFn    lock;
  char          *buffer;
  uint32_t      buffer_size;
#ifdef LOG_HAS_FILE_OUT
  FILE          *fp;
#endif
  log_level_t   level;
  bool          quiet;
} g_LOG = {
  .udata        = NULL,
  .lock         = dummy_lock,
  .buffer       = NULL,
  .buffer_size  = 0,
#ifdef LOG_HAS_FILE_OUT
  .fp           = NULL,
#endif
  .level        = LOG_ERROR,
  .quiet        = false, 
};

#ifdef LOG_HAS_FILE_OUT
static const char *level_names[] = {
  "SUDO ", "FATAL", "ERROR", "WARN ", "INFO ", "DEBUG", "TRACE",
};
#endif

#ifdef LOG_USE_COLOR
static const char *level_colors[] = {
  "\x1b[33m", "\x1b[35m", "\x1b[31m", "\x1b[33m", "\x1b[32m", "\x1b[36m", "\x1b[94m",
};
#endif


void log_set_udata(const handle_t udata)
{
  g_LOG.udata = udata;
}


void log_set_lock(log_LockFn fn)
{
  g_LOG.lock = fn;
}

#ifdef LOG_HAS_FILE_OUT
void log_set_fp(FILE *fp)
{
  g_LOG.fp = fp;
}
#endif

void log_set_level(const log_level_t level)
{
  g_LOG.level = level;
}


void log_set_quiet(const bool enable)
{
  g_LOG.quiet = enable;
}

bool log_set_buffer(char* buffer, const uint32_t buffer_size)
{
  if ( !(buffer && (buffer_size>0)) ) {
    return false;
  }

  g_LOG.buffer      = buffer;
  g_LOG.buffer_size = buffer_size;

  return true;
}

uint32_t log_log(
  const char* prefix,
  const log_level_t  level,
  const char *file, const int line,
  const char *fmt, va_list args)
{
  uint32_t idx = 0;

  if ( level>g_LOG.level ) {
    return 0;
  }

  /* Acquire lock */
  g_LOG.lock(g_LOG.udata, true);

#ifdef LOG_HAS_DATETIME
  /* Get current time */
  time_t t = time(NULL);
  struct tm *lt = localtime(&t);
#endif

#ifdef LOG_STRIP_FILE_PATH
  if ( (file=strrchr(file, LOG_STRIP_FILE_PATH)) ) {
    file++;
  }
#endif
  /* Log to stderr */
  if ( (!g_LOG.quiet) && g_LOG.buffer )
  {
    char* dst = g_LOG.buffer;
    const uint32_t dst_size = g_LOG.buffer_size;

    if ( prefix ) {
      idx += snprintf(dst+idx, dst_size-idx, "[%s] ", prefix);
      printf("idx(%u) %s\r\n", idx, dst);
    }
#if 0
#ifdef LOG_HAS_DATETIME
    idx += strftime(dst, dst_size, "[%H:%M:%S", lt);
    dst[idx] = '\0';
#else
    dst[idx++] = '['; dst[idx] = '\0';
#endif
#ifdef LOG_USE_COLOR
    idx += snprintf(dst+idx, dst_size-idx,
      "%s%-5s\x1b[0m \x1b[90m] %s:%d:\x1b[0m ",
      level_colors[level], level_names[level], file, line);
#else
    idx += snprintf(dst+idx, dst_size-idx,
      "%-5s] %s:%d: ", level_names[level], file, line);
#endif
#endif
    //idx += vsnprintf(dst+idx, dst_size-idx, fmt, args);
    idx += snprintf(dst+idx, dst_size-idx, "\r\n");
    dst[idx] = '\0';
    printf("idx(%u) %s\r\n", idx, dst);
    //fflush(stdout);
  }

#ifdef LOG_HAS_FILE_OUT
  /* Log to file */
  if ( g_LOG.fp )
  {
    char buf[32];
    va_list fargs;
#ifdef LOG_HAS_DATETIME
    buf[strftime(buf, sizeof(buf), "[%Y-%m-%d %H:%M:%S", lt)] = '\0';
#else
    buf[0] = '['; buf[1] = '\0';
#endif
    fprintf(g_LOG.fp, "%s %-5s] %s:%d: ", buf, level_names[level], file, line);
    va_start(fargs, fmt);
    vfprintf(g_LOG.fp, fmt, fargs);
    va_end(fargs);
    fprintf(g_LOG.fp, "\n");
    fflush(g_LOG.fp);
  }
#endif

  /* Release lock */
  g_LOG.lock(g_LOG.udata, false);

  return idx;
}

