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
};

__HYDRUS_PLATFORM_END

