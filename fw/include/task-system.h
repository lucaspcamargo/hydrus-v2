#pragma once

#include "hydrus-config.h"
#include "hydrus-traits.h"

#include "task.h"
#include "platform/logger.h"


__HYDRUS_BEGIN

class SystemTask : public Task
{
public:
    
    typedef Traits< SystemTask > Tr;
    
    SystemTask() : Task()
    {
        
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


