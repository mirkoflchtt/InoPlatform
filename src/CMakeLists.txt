cmake_minimum_required(VERSION 2.8)

set(LIB_CPP_SOURCES
    InoAdvButton.cpp
    InoCRC.cpp
    InoDateTime.cpp
    InoEventHandler.cpp
    InoHalAdapter.cpp
    InoLFSR.cpp
    # InoLog.cpp
    # InoOTA.cpp
    InoPinOut.cpp
    InoPlatform.cpp
    InoSensorObj.cpp
    # InoSensorTemperature.cpp
    InoStdButton.cpp)

set(LIB_C_SOURCES
    event_manager.c
    ino_rbuffer.c
    log.c)

add_library(InoPlatform STATIC ${LIB_CPP_SOURCES} ${LIB_C_SOURCES})
target_link_libraries(InoPlatform m)

install_static(InoPlatform)
