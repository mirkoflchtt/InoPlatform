#ifndef INO_LOOPER_H
#define INO_LOOPER_H
#include <vector>
#include <Ticker.h>

#include "InoMyConfig.h"
#include "InoAssert.h"
#include "InoTypes.h"
#include "InoFlags.h"

INO_NAMESPACE

typedef ino_bool (*looper_loop_fn)(ino_handle fn_arg);

class Looper {
public:
  Looper(void);
  ~Looper(void);

  ino_bool pushLoop(
    looper_loop_fn fn,
    ino_handle fn_arg=INO_HANDLE_NULL,
    const ino_interval interval_ms=0);

  ino_bool popLoop(
    looper_loop_fn fn,
    ino_handle fn_arg=INO_HANDLE_NULL);

  ino_bool init(void);

  ino_bool loop(void);

private:
  class LoopEntry {
  public:
    LoopEntry(looper_loop_fn fn, ino_handle fn_arg, const ino_interval interval_ms) :
      m_fn(fn),
      m_fn_arg(fn_arg),
      m_ticker(NULL)
    {
      INO_FLAG_CLEAR(m_flags)

      if (interval_ms>0) {
        m_ticker = new Ticker();
        INO_ASSERT(m_ticker)
      }
      
      if (m_ticker) {
        INO_FLAG_SET(m_flags, 0x1)
        m_ticker->attach_ms(interval_ms, trigger);
      }
    }

    static
    void trigger(void) {
      INO_FLAG_SET(m_flags, 0x1)
    }

    ino_bool equal(looper_loop_fn fn, ino_handle fn_arg)
    {
      return (m_fn==fn && m_fn_arg==fn_arg);
    }

    ino_bool loop(void)
    {
      if (!m_ticker || INO_FLAG_GET(m_flags, 0x1)) {
        INO_FLAG_UNSET(m_flags, 0x1)
        return m_fn(m_fn_arg);
      }
      return true;
    }

  private:
    looper_loop_fn              m_fn;
    ino_handle                  m_fn_arg;
    static ino_flags            m_flags;
    Ticker*                     m_ticker;
  };

  std::vector<LoopEntry>        m_loop_fn;
};


INO_API_DECLARE

INO_API_DECLARE_END

INO_NAMESPACE_END

#endif    /*INO_LOOPER_H*/
