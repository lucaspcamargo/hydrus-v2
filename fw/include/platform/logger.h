#pragma once

#include "hydrus-config.h"

__HYDRUS_PLATFORM_BEGIN

class Logger
{
public:
    enum Level
    {
        UNSPECIFIED,
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };
    
    static void log( const char * where, const char * what, Level lvl = INFO);    
    static const char * log_severity_str(Level lvl);
    
    class LogListener
    {
    public: 
        virtual void log_received( const char * where, const char * what, Level lvl = INFO) = 0;
    };
    
    static void register_listener( LogListener * ll );
    
};

__HYDRUS_PLATFORM_END

