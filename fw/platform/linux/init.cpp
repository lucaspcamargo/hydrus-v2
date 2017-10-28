#include "hydrus-config.h"

#include <platform/logger.h>
#include <platform/mutex.h>

#include <stdio.h>
#include <sys/mman.h>

__HYDRUS_PLATFORM_BEGIN

    extern Mutex *_linux_logger_lock;

    void preinit()
    {
        _linux_logger_lock = new Mutex();    
    }
    
    void init( int argc, char **argv )
    {   
        // here we initialize the specifics of the platform
        Logger::log("init", "locking memory");
        if(mlockall( MCL_CURRENT
#ifndef __arm__
 | MCL_ONFAULT
#endif
        ))
        {
            Logger::log("init", "error locking memory", Logger::ERROR);
            perror("locking memory");
        }
        
    }

__HYDRUS_PLATFORM_END
