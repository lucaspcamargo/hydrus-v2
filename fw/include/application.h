#include <unistd.h>
#pragma once

#include "hydrus-config.h"
#include "test/dummytask.h"
#include "periodicthread.h"

#include "task-navigation.h"
#include "task-sensing.h"
#include "task-system.h"

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
        
        Blackboard::init();
        
        PeriodicThread sys( new SystemTask() );
        PeriodicThread sen( new SensingTask() );
        PeriodicThread nav( new NavigationTask() );
                
        nav.join();
        sen.join();
        sys.join();
        
        P::Logger::log("application", "joined all threads");      
        P::Logger::log("application", "finished");        
        
        return 0;
    }
    
private:
        
};

__HYDRUS_END
