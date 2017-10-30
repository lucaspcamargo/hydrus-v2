#include "platform/logger.h"

#include <iostream>
#include "platform/mutex.h"

#include <vector>

__HYDRUS_PLATFORM_BEGIN

Mutex *_linux_logger_lock = 0;
std::vector<Logger::LogListener*> _logger_lls;

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
    
    for( auto a = _logger_lls.begin(); a != _logger_lls.end(); a++ )
    {
        (*a)->log_received(where, what, lvl);
    }
}

const char * Logger::log_severity_str(Logger::Level lvl) 
{
    return _log_level_to_str(lvl);
}


void Logger::register_listener( Logger::LogListener * ll )
{
    _logger_lls.push_back(ll);
}


__HYDRUS_PLATFORM_END
