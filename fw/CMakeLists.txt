cmake_minimum_required(VERSION 3.5)
set (CMAKE_CXX_STANDARD 11) # this is a C++11 project

set( HYDRUS_PLATFORM "linux" CACHE STRING "The platform to build the drone firmware for" )

## PLATFORM SETTINGS

include( platform/${HYDRUS_PLATFORM}/${HYDRUS_PLATFORM}.cmake )

## 3rdparty




## CONFIG HEADER

configure_file (
  "${PROJECT_SOURCE_DIR}/include/hydrus-config.h.in"
  "${PROJECT_BINARY_DIR}/hydrus-config.h"
  )
include_directories( ${PROJECT_BINARY_DIR} ./include )


## SOURCE FILES

set( SRCS 
    src/main.cpp    
    
    src/blackboard.cpp
    src/dev-gps.cpp
    src/periodicthread.cpp
    )

    
## TARGET

# EPOS: This part can change later
# Idea: generate a static object with a well-defined interface, with an 
# epos-specific shim that leverages that interface.

add_executable( hydrusv2_fw ${SRCS} ${HYDRUS_P_SRCS} )
if( HYDRUS_P_LIBS )
    target_link_libraries( hydrusv2_fw ${HYDRUS_P_LIBS} -l:libnmea.a -ldl )
endif( HYDRUS_P_LIBS )
