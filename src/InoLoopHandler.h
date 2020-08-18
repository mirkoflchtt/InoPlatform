#ifndef INO_LOOP_HANDLER_H
#define INO_LOOP_HANDLER_H
#include <vector>
#include <Ticker.h>

#include "InoMyConfig.h"
#include "InoTypes.h"
#include "InoEvents.h"

INO_NAMESPACE

typedef ino_bool (*loop_handler_cb)(ino_handle cb_arg);

class LoopEntry {
public:
  LoopEntry(loop_handler_cb cb, ino_handle cb_arg, const ino_interval interval_ms);
  
  ~LoopEntry(void);

  void trigger(void);
  
  ino_bool is_equal(const LoopEntry& entry);

  ino_bool loop(void);

private:
  loop_handler_cb       m_cb;
  ino_handle            m_cb_arg;
  ino_flags             m_flags;
  Ticker*               m_ticker;
};

class LoopHandler {
public:
  LoopHandler(void);
  ~LoopHandler(void);

  ino_bool pushLoop(
    loop_handler_cb cb,
    ino_handle cb_arg=INO_HANDLE_NULL,
    const ino_interval interval_ms=0);

  ino_bool popLoop(
    loop_handler_cb cb,
    ino_handle cb_arg=INO_HANDLE_NULL);

  ino_bool init(void);

  ino_bool loop(void);

private:
  std::vector<LoopEntry>  m_entry;
};


INO_API_DECLARE

INO_API_DECLARE_END

INO_NAMESPACE_END

#endif    /*INO_LOOP_HANDLER_H*/
