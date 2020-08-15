#ifndef INO_PLATFORM_H
#define INO_PLATFORM_H

#include "InoMyConfig.h"
#include "InoTypes.h"
#include "InoAssert.h"
#include "InoBase64.h"
#include "InoCRC.h"
#include "InoDateTime.h"
#include "InoEventHandler.h"
#include "InoFirmware.h"
#include "InoFlags.h"
#include "InoLFSR.h"
#include "InoLog.h"
#include "InoLooper.h"
#include "InoHalAdapter.h"
#include "InoPinOut.h"
#include "InoAdvButton.h"
#include "InoStdButton.h"

#ifndef HAS_X86
#include "InoOTA.h"
#include "InoSensorTemperature.h"
#endif

#endif    /*INO_PLATFORM_H*/
