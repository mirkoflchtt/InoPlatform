//#include <stdio.h>
#include <vector>
#include <Ticker.h>
#include "InoLooper.h"
#include "InoHalAdapter.h"


INO_NAMESPACE

Looper::Looper(void) :
m_loop_fn()
{
}

Looper::~Looper(void)
{
}

ino_bool Looper::pushLoop(
  looper_loop_fn fn,
  ino_handle fn_arg,
  const ino_interval interval_ms)
{
  if (!fn) return false;
  m_loop_fn.push_back(LoopEntry(fn , fn_arg, interval_ms));
  return true;
}

ino_bool Looper::popLoop(
  looper_loop_fn fn,
  ino_handle fn_arg)
{
  if (!fn) return false;
  for (std::vector<LoopEntry>::iterator it=m_loop_fn.begin(); 
       it!=m_loop_fn.end(); it++ )
  {
    if (it->equal(fn, fn_arg)) {
      m_loop_fn.erase(it);
      return true;
    }
  }
  return false;
}

ino_bool Looper::init(void)
{
  m_loop_fn.clear();
  return true;
}

ino_bool Looper::loop(void)
{
  ino_bool ok = true;
  for (std::vector<LoopEntry>::iterator it=m_loop_fn.begin();
       it!=m_loop_fn.end(); it++)
  {
    ok &= it->loop();
  }
  return ok;
}

INO_API_DECLARE

INO_API_DECLARE_END

INO_NAMESPACE_END
