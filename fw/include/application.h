#include <unistd.h>
#pragma once

#include "hydrus-config.h"
#include "test/dummytask.h"
#include "periodicthread.h"
#include "navigation.h"

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
        
        PeriodicThread dt( new DummyTask() );
        PeriodicThread nav( new Navigation() );
        usleep( 100000u );
        
        nav.join();
        dt.join();
        P::Logger::log("application", "joined all threads"); 
        
        
        P::Logger::log("application", "finished");        
        
        return 0;
    }
    
private:
        
};

__HYDRUS_END
