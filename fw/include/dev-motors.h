#pragma once

#include "hydrus-config.h"
#include "platform/pwm.h"
#include "blackboard.h"
#include <cmath>

__HYDRUS_BEGIN

class Motors
{
public:
    
    typedef Traits< Motors > Tr;
  
    Motors():
    m_left(0),
    m_right(1)
    {
        
    }
    
    int calculateCycle(float alpha)
    {
        uint32_t maxValue = Tr::MAX_PERIOD;
        uint32_t minValue = Tr::MIN_PERIOD + Tr::MIN_DEADBAND;

        uint32_t diff = maxValue - minValue;
        
        return minValue + fmin(1,fmax(0,alpha)) * diff;
    }

    void tick()
    {

        uint32_t pwmL = Tr::MIN_PERIOD,
            pwmR = Tr::MIN_PERIOD;
            
        BBro->trans.begin();    
        if(BBro->nav.motor.enabled || !(Tr::SIM_MODE_DISABLES_MOTORS && BBro->nav.simMode))
        {
            pwmL = calculateCycle(BBro->nav.motor.spdL * Tr::MOTOR_FACTOR_L);
            pwmR = calculateCycle(BBro->nav.motor.spdR * Tr::MOTOR_FACTOR_R);
        }
        BBro->trans.end();
        
        m_left.set(pwmL);
        m_right.set(pwmR);

    }
    
private:
    P::PWM m_left;
    P::PWM m_right;
    
};

__HYDRUS_END
