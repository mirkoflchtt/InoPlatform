cmake_minimum_required(VERSION 2.8)
project(InoPlatform)

# list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

################################################################################
## MACRO DEFINITION SECTION

macro(install_bin)
  install(TARGETS ${ARGN} EXPORT ino-targets
          RUNTIME DESTINATION ${DEFAULT_INSTALL_PATH}/bin)
endmacro()

macro(install_static)
#if ( BUILD_STATIC_LIBS )
  message("## [install_static] : ${DEFAULT_INSTALL_PATH}/lib/static")
  install(TARGETS ${ARGN} EXPORT ino-targets
          ARCHIVE DESTINATION ${DEFAULT_INSTALL_PATH}/lib/static)
#endif ()
endmacro()

macro(install_shared)
#if ( BUILD_SHARED_LIBS )
  install(TARGETS ${ARGN} EXPORT ino-targets
          LIBRARY DESTINATION ${DEFAULT_INSTALL_PATH}/lib
          RUNTIME DESTINATION ${DEFAULT_INSTALL_PATH}/lib)
#endif ()
endmacro()

macro(install_headers)
  install(FILES ${ARGN} DESTINATION ${DEFAULT_INSTALL_PATH}/include)
#install(    TARGETS ${headers} EXPORT ino-targets
#            PUBLIC_HEADER DESTINATION ${DEFAULT_INSTALL_PATH}/include )
endmacro()

macro(use_c99)
  if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
      set (CMAKE_C_FLAGS "-std=gnu99 ${CMAKE_C_FLAGS}")
    endif ()
  else ()
    set (CMAKE_C_STANDARD 99)
  endif ()
endmacro(use_c99)

macro(use_c11)
  if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
      set (CMAKE_C_FLAGS "-std=gnu11 ${CMAKE_C_FLAGS}")
    endif ()
  else ()
    set (CMAKE_C_STANDARD 11)
  endif ()
endmacro(use_c11)

################################################################################
## OPTIONS SECTION
# Code versioning
option(WITH_CODE_VERSIONING " Enable code versioning during compilation " ON)
message("-- Enable code versioning DEFAULT: ${WITH_CODE_VERSIONING}")

option(WITH_BUILTIN_TESTS " Enable builtin tests " ON)
message("-- Enable builtin test DEFAULT: ${WITH_BUILTIN_TESTS}")

# Enable address sanitizer
## option(WITH_SANITIZER " Enable address sanitizer options " OFF)
## message("-- Enable address sanitizer options DEFAULT : ${WITH_SANITIZER}")

################################################################################
## ENVIRONMENT SETUP

# set(CMAKE_BUILD_TYPE Debug)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if ( ${CMAKE_BUILD_TYPE} STREQUAL Release )
  message( "\n### BUILD_TYPE: RELEASE")
endif ()

if ( ${CMAKE_BUILD_TYPE} STREQUAL RelWithDebInfo )
  message( "\n### BUILD_TYPE: PROFILING" )
endif ()

if ( ${CMAKE_BUILD_TYPE} STREQUAL Debug )
  message( "\n### BUILD_TYPE: DEBUG" )
  set(WITH_DEBUG_LIB ON CACHE BOOL "Enable WITH_DEBUG_LIB option" FORCE)
endif ()

if (CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
  message("\n### X86 Target Detected")
  add_definitions(-DHAS_X86)
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_definitions(-fpermissive)

# set(DEFAULT_INSTALL_PATH ${PROJECT_SOURCE_DIR})
# set(DEFAULT_INSTALL_PATH ${CMAKE_SYSTEM_PROCESSOR})
set(DEFAULT_INSTALL_PATH ".")

use_c99()
#use_c11()

set(CLANG CMAKE_C_COMPILER_ID STREQUAL "Clang")

if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
else()
  set(LINUX FALSE)
endif()

if (CLANG OR CMAKE_COMPILER_IS_GNUCXX)
  add_definitions("-Wall -Wextra -Wfloat-conversion -Wno-unknown-pragmas"
                  "-Wno-unused-parameter -Wno-missing-field-initializers")
endif()

if (WITH_SANITIZER)
  if (NOT WIN32 AND ${CMAKE_BUILD_TYPE} STREQUAL Debug)
    add_definitions("-fsanitize=address -fno-omit-frame-pointer")
    link_libraries("-fsanitize=address")
  endif()
endif()

if (MSVC)
  add_definitions("-D_CRT_SECURE_NO_WARNINGS")
endif()

## if (WITH_CODE_VERSIONING)
if (0)
  include(GetGitRevisionDescription)
  get_git_head_revision(GIT_REFSPEC GIT_SHA1)
  if(GIT_SHA1)
    configure_file("ino_platform_config.h.in"
                   "ino_platform_config.h" @ONLY)
    add_definitions("-DINO_HAS_PLATFORM_CONFIG")
  endif()
endif()

if (WITH_BUILTIN_TESTS)
  add_definitions("-DHAS_INO_BUILTIN_TESTS")
endif()

message("-- CMAKE_SYSTEM_NAME      : ${CMAKE_SYSTEM_NAME}")
message("-- CMAKE_SYSTEM_PROCESSOR : ${CMAKE_SYSTEM_PROCESSOR}")

# Avoid issues with missing CMAKE_BUILD_TYPE; see:
# https://stackoverflow.com/questions/24460486/cmake-build-type-not-being-used-in-cmakelists-txt
set(CMAKE_C_FLAGS_DEBUG
    "${CMAKE_C_FLAGS_DEBUG} -DHAS_INO_ASSERT -DHAS_INO_DEBUG -DHAS_INO_BUILTIN_TESTS")

include_directories(src x86/include)

add_subdirectory(src)
# add_subdirectory(examples)

add_subdirectory(x86)
