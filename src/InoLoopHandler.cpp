

#include "InoAssert.h"
#include "InoFlags.h"
#include "InoLoopHandler.h"
#include "InoHalAdapter.h"


INO_NAMESPACE

INO_DECLARE_STATIC
void GlobalLoopHandlerCallbackFunction(LoopEntry* entry)
{
  INO_ASSERT(entry)
  entry->trigger();
}

/****************************************************************************/
LoopEntry::LoopEntry(
  loop_handler_cb cb, ino_handle cb_arg, const ino_interval interval_ms=0) :
m_cb(cb),
m_cb_arg(cb_arg),
m_ticker(NULL)
{
  INO_FLAG_CLEAR(m_flags)

  if (interval_ms>0) {
    m_ticker = new Ticker();
    INO_ASSERT(m_ticker)
  }

  if (m_ticker) {
    trigger();
    m_ticker->attach_ms(interval_ms, GlobalLoopHandlerCallbackFunction, this);
  }
}

LoopEntry::~LoopEntry(void)
{
  delete m_ticker;
  m_ticker = NULL;
}

void LoopEntry::trigger(void)
{
  INO_FLAG_SET(m_flags, 0x1)
}

ino_bool LoopEntry::is_equal(const LoopEntry& entry)
{
  return (m_cb==entry.m_cb && m_cb_arg==entry.m_cb_arg);
}
  
ino_bool LoopEntry::loop(void)
{
  if (!m_ticker || INO_FLAG_GET(m_flags, 0x1)) {
    INO_FLAG_UNSET(m_flags, 0x1)
    return m_cb(m_cb_arg);
  }
  return true;
}

/****************************************************************************/
LoopHandler::LoopHandler(void) :
m_entry()
{
}

LoopHandler::~LoopHandler(void)
{
  m_entry.clear();
}

ino_bool LoopHandler::pushLoop(
  loop_handler_cb cb,
  ino_handle cb_arg,
  const ino_interval interval_ms)
{
  if (!cb) return false;
  m_entry.push_back(LoopEntry(cb , cb_arg, interval_ms));
  return true;
}

ino_bool LoopHandler::popLoop(
  loop_handler_cb cb,
  ino_handle cb_arg)
{
  if (!cb) return false;
  for (std::vector<LoopEntry>::iterator it=m_entry.begin(); 
       it!=m_entry.end(); it++)
  {
    if (it->is_equal(LoopEntry(cb, cb_arg))) {
      m_entry.erase(it);
      return true;
    }
  }
  return false;
}

ino_bool LoopHandler::init(void)
{
  m_entry.clear();
  return true;
}

ino_bool LoopHandler::loop(void)
{
  ino_bool ok = true;
  for (std::vector<LoopEntry>::iterator it=m_entry.begin();
       it!=m_entry.end(); it++)
  {
    ok &= it->loop();
  }
  return ok;
}

INO_API_DECLARE

INO_API_DECLARE_END

INO_NAMESPACE_END
