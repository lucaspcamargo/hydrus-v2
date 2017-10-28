
set( HYDRUS_P_HAS_STDLIB 1 )

set( PP platform/linux ) # platform prefix
set( HYDRUS_P_SRCS ${PP}/main.cpp  ${PP}/geomag.cpp ${PP}/init.cpp ${PP}/i2c.cpp ${PP}/threading.cpp  ${PP}/hardware.cpp ${PP}/logger.cpp ${PP}/timer.cpp )
set( HYDRUS_P_LIBS -lpthread )
