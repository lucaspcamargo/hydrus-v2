#pragma once

#include "hydrus-config.h"

__HYDRUS_PLATFORM_BEGIN

/**
 * @brief Generic GPIO access class. Refer to the documentation for pins and capabilities.
*/

class GPIO 
{
public:
        
    enum Direction
    {
      INPUT,
      OUTPUT
    };
    
    enum Flags
    {
      PULLUP = 1,
      PULLDOWN = 2,
      OPEN_COLLECTOR = 4,
    };
    
    enum Level
    {
      LOW,
      HIGH
    };
    
    GPIO( unsigned int pin, Direction direction, Flags caps );
    
    void configure( Direction direction, Flags flags );
    
    Level get();
    void set( Level l );
    
};

__HYDRUS_PLATFORM_END
