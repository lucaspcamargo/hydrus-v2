#pragma once

#include "hydrus-config.h"
#include "test/dummytask.h"
#include "periodicthread.h"

__HYDRUS_BEGIN

class Application
{
public:
        
    
    Application( )
    {
        
    }
    
    int exec()
    {
        
        P::Logger::log("application", "started");   
        
        ( new PeriodicThread(new DummyTask()) )->join();
        
        P::Logger::log("application", "joined dummy task");        
        
        return 0;
    }
    
private:
        
};

__HYDRUS_END
