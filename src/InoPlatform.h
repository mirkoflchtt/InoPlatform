#ifndef __INO_PLATFORM_H__
#define __INO_PLATFORM_H__

#include "InoMyConfig.h"
#include "InoTypes.h"
#include "InoAssert.h"
#include "InoCRC.h"
#include "InoDateTime.h"
#include "InoEventHandler.h"
#include "InoFirmware.h"
#include "InoFlags.h"
#include "InoLFSR.h"
#include "InoLog.h"
#include "InoHalAdapter.h"
#include "InoPinOut.h"
#include "InoAdvButton.h"
#include "InoStdButton.h"

#ifndef HAS_X86
#include "InoOTA.h"
#include "InoSensorTemperature.h"
#endif

#endif    /*__INO_PLATFORM_H__*/
