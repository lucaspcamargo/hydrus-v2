#pragma once

#include "hydrus-config.h"
#include "hydrus-traits.h"
#include "hydrus-math.h"

#include "task.h"
#include "platform/logger.h"

#include "dev-magnetometer.h"
#include "dev-adc.h"
#include "blackboard.h"

#include "stdio.h"

__HYDRUS_BEGIN

class SensingTask : public Task
{
public:
    
    typedef Traits< SensingTask > Tr;
    
    SensingTask() : 
    Task(),
//     m_adc(P::I2CBus::getBusInstance(0)),
    m_mag(P::I2CBus::getBusInstance(0))
    {
        
    }        
    
    virtual bool tick() override 
    {
        m_mag.sample();
        
        /*float adcValues[4];
        for(int i = 0; i < 4; i++)
            adcValues[i] = m_adc.get_volts(i);
            
        // make sense of adc values
        float waterPH = adcValues[0];
        float waterTemp = adcValues[1];
        float waterTurb = adcValues[2];
        float battVolts = adcValues[3];*/
        
//         fprintf(stderr, "ADC VALUES %f %f %f %f \n", adcValues[0], adcValues[1],  adcValues[2],  adcValues[3] );
        
        // write values to blackboard
        BB->trans.begin();
        
        BB->sensors.imuHeading = m_mag.heading();
        fprintf(stderr, "HEADING %f %f %f %f \n", BB->sensors.imuHeading, m_mag.x(), m_mag.y(), m_mag.z());
//         BB->sensors.waterPH = waterPH;
//         BB->sensors.waterTemp = waterTemp;
//         BB->sensors.waterTurb = waterTurb;
//         BB->sys.battVoltage = battVolts;
                
//         BB->sys.battLevel = hClamp((battVolts - Tr::MIN_BATT_V) / (Tr::MAX_BATT_V - Tr::MIN_BATT_V));
        BB->trans.end();
        
        return true; // keep running
    }
    
    virtual uint64_t period_us() override 
    {        
        return Tr::THREAD_INTERVAL_us;
    }
    
    
private:
//     ADC m_adc;
    Magnetometer m_mag;
};

__HYDRUS_END

