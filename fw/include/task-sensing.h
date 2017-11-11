#pragma once

#include "hydrus-config.h"
#include "hydrus-traits.h"
#include "hydrus-math.h"

#include "task.h"
#include "platform/logger.h"

#include "dev-magnetometer.h"
#include "dev-adc.h"
#include "blackboard.h"
#include "sensors.h"

// #include "stdio.h"

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
        static PHSensor ph_s;
        static TemperatureSensor temp_s;
        static TurbiditySensor turb_s;
        static BatterySensor batt_s;
                
        m_mag.sample();
        
        float adcValues[4];
        for(int i = 0; i < 4; i++)
        {
            adcValues[i] = m_adc.get_volts(i);
        }
        
        // make sense of adc values
        float waterPH   = ph_s.convert(adcValues[0]);
        float waterTemp = temp_s.convert(adcValues[1]);
        float waterTurb = turb_s.convert(adcValues[2]);
        float battVolts = batt_s.convert(adcValues[3]);
        
        
        // write values to blackboard
        BB->trans.begin();
        
        BB->sensors.imuHeading = m_mag.heading();
        BB->sensors.waterPH = waterPH;
        BB->sensors.waterTemp = waterTemp;
        BB->sensors.waterTurb = waterTurb;
        BB->sys.battVoltage = battVolts;
                
        BB->sys.battLevel = hClamp((battVolts - Tr::MIN_BATT_V) / (Tr::MAX_BATT_V - Tr::MIN_BATT_V));
        BB->trans.end();
        
//         fprintf(stderr, "ADC VALUES %f %f %f %f \n", adcValues[0], adcValues[1],  adcValues[2],  adcValues[3] );
//         fprintf(stderr, "HEADING %f %f %f %f \n", BB->sensors.imuHeading, m_mag.x(), m_mag.y(), m_mag.z());
        
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

