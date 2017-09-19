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
      INIT_OUTPUT_AS_HIGH = 8,
    };
    
    enum Level
    {
      LOW,
      HIGH,
      INVALID
    };
    
    GPIO( unsigned int pin, Direction direction, Flags caps );
    
    bool configure( Direction direction, Flags flags );
    
    Level get();
    void set( Level l );
    
private:
    void *_p;
};

__HYDRUS_PLATFORM_END
