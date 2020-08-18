#ifndef INO_EVENTS_H
#define INO_EVENTS_H
#pragma once

#include "InoTypes.h"

INO_NAMESPACE

class Events {
public:
  enum {
    GOT_TEMPERATURE_HUMIDITY  = 0x1U,
    EVENT_LAST                = 0x100U,
  };

private:

};

INO_API_DECLARE

INO_API_DECLARE_END

INO_NAMESPACE_END

#endif    /*INO_EVENTS_H*/
