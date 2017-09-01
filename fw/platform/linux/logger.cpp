#include "platform/logger.h"

#include <iostream>
#include "platform/mutex.h"

__HYDRUS_PLATFORM_BEGIN

/*
namespace Linux {
  
    Logger * log;
    
};*/

Mutex *_linux_logger_lock = 0;

const char* _log_level_to_str ( Logger::Level lvl )
{
    switch( lvl )
    {
        case Logger::UNSPECIFIED:
            return "";
            
        case Logger::DEBUG:
            return "DBG";
            
        case Logger::INFO:
            return "INF";
            
        case Logger::WARNING:
            return "WRN";
            
        case Logger::ERROR:
            return "ERR";
    }
}

void Logger::log( const char * where, const char * what, Logger::Level lvl )
{
    MutexLock l( *_linux_logger_lock );
    std::cerr << '[' << where << "] " << _log_level_to_str(lvl) << ": " << what << std::endl;
}



__HYDRUS_PLATFORM_END
