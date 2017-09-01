#pragma once

#include "hydrus-config.h"
#include "task.h"
#include "platform/logger.h"

__HYDRUS_BEGIN

class Navigation : public Task
{
public:
    
    static const uint64_t NAV_PERIOD_us = 500000u;
    
    Navigation() : Task()
    {
        
    }
        
    
    virtual bool tick() override 
    {
        P::Logger::log("nav", "arriving for duty");
        
        return true; // keep running
    }
    
    virtual uint64_t period_us() override 
    {        
        return NAV_PERIOD_us;
    }
    
    
private:
    
};

__HYDRUS_END
