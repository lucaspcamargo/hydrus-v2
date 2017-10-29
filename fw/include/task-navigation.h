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
        bzero(&p_gpsm, sizeof(p_gpsm));
    }
        
    
    virtual bool tick() override 
    {        
        if(GPS::has_message())
        {
            GPS::Message m = GPS::get_message_copy();
            
        }
        
        return true; // keep running
    }
    
    virtual uint64_t period_us() override 
    {        
        return Tr::THREAD_INTERVAL_us;
    }
    
    
private:
    GPS::Message p_gpsm;
};

__HYDRUS_END
