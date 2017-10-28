#pragma once

#include "hydrus-config.h"
#include "task.h"
#include "platform/logger.h"

#include "dev-gps.h"

__HYDRUS_BEGIN

class NavigationTask : public Task
{
public:
    
    typedef Traits< NavigationTask > Tr;
    
    NavigationTask() : Task()
    {
        GPS::init();        
    }
        
    
    virtual bool tick() override 
    {        
        
        return true; // keep running
    }
    
    virtual uint64_t period_us() override 
    {        
        return Tr::THREAD_INTERVAL_us;
    }
    
    
private:
    
};

__HYDRUS_END
