#pragma once

#include "hydrus-config.h"
#include "hydrus-traits.h"

#include "task.h"
#include "platform/logger.h"

#include "dev-magnetometer.h"

__HYDRUS_BEGIN

class SensingTask : public Task
{
public:
    
    typedef Traits< SensingTask > Tr;
    
    SensingTask() : 
    Task(),
    m_mag(P::I2CBus::getBusInstance(0))
    {
        
    }        
    
    virtual bool tick() override 
    {
        
        m_mag.sample();
        
        return true; // keep running
    }
    
    virtual uint64_t period_us() override 
    {        
        return Tr::THREAD_INTERVAL_us;
    }
    
    
private:
    Magnetometer m_mag;
};

__HYDRUS_END

