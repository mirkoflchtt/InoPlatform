cmake_minimum_required(VERSION 2.8)

set(LIB_CPP_SOURCES
    InoAdvButton.cpp
    InoBase64.cpp
    InoCRC.cpp
    InoDateTime.cpp
    InoEventHandler.cpp
    InoHalAdapter.cpp
    InoLFSR.cpp
    InoLog.cpp
    # InoOTA.cpp
    InoPinOut.cpp
    InoPlatform.cpp
    InoSensorObj.cpp
    # InoSensorTemperature.cpp
    InoStdButton.cpp)

set(LIB_C_SOURCES
    ino_rbuffer.c
    log.c)

add_library(InoPlatformStatic STATIC ${LIB_CPP_SOURCES} ${LIB_C_SOURCES})
target_link_libraries(InoPlatformStatic m)
install_static(InoPlatformStatic)

add_library(InoPlatform SHARED ${LIB_CPP_SOURCES} ${LIB_C_SOURCES})
target_link_libraries(InoPlatform m)
install_shared(InoPlatform)

