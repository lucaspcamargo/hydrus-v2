#pragma once

#include "hydrus-config.h"


__HYDRUS_PLATFORM_BEGIN

class PWM
{
public:
    PWM(int channel);    
    ~PWM();
    
    void set(float alpha)    // set by alpha value [0..1]
    {
        set( (uint32_t) (((1 << bits()) - 1) * alpha) );
    }
    
    void set(uint32_t value); // set by integer number
    
    int bits() const; // get number of pwm value bits
    
private:
    int _ch;
    void *_p;
};

__HYDRUS_PLATFORM_END
