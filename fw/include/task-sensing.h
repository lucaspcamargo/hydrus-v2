#pragma once

#include "hydrus-config.h"
#include "hydrus-traits.h"

#include "task.h"
#include "platform/logger.h"

#include "dev-magnetometer.h"
#include "dev-adc.h"

__HYDRUS_BEGIN

class SensingTask : public Task
{
public:
    
    typedef Traits< SensingTask > Tr;
    
    SensingTask() : 
    Task(),
    m_adc(P::I2CBus::getBusInstance(0)),
    m_mag(P::I2CBus::getBusInstance(0))
    {
        
    }        
    
    virtual bool tick() override 
    {
        m_mag.sample();
        
        float adcValues[4];
        for(int i = 0; i < 4; i++)
            adcValues[i] = m_adc.get_volts(i);
            
        return true; // keep running
    }
    
    virtual uint64_t period_us() override 
    {        
        return Tr::THREAD_INTERVAL_us;
    }
    
    
private:
    ADC m_adc;
    Magnetometer m_mag;
};

__HYDRUS_END

