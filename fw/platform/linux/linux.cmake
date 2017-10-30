
set( HYDRUS_P_HAS_STDLIB 1 )

set( PP platform/linux ) # platform prefix
set( HYDRUS_P_SRCS 
            ${PP}/geomag.cpp 
            ${PP}/hardware.cpp 
            ${PP}/init.cpp 
            ${PP}/i2c.cpp 
            ${PP}/logger.cpp 
            ${PP}/main.cpp  
            ${PP}/pwm.cpp 
            ${PP}/threading.cpp  
            ${PP}/timer.cpp 
            )
set( HYDRUS_P_LIBS -lpthread )

if( CMAKE_SYSTEM_PROCESSOR MATCHES "arm" )
    set( HYDRUS_P_LIBS -lpigpio -lrt )
endif( CMAKE_SYSTEM_PROCESSOR MATCHES "arm" )
