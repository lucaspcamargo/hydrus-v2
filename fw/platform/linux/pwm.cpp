#include "platform/pwm.h"

#define PWMBITS 24

#ifdef __arm__

#include <pigpio.h>
// NOTE: ON RASPBERRY PI
// PWM0 -> pin 18 -> l
// PWM1 -> pin 19 -> r

__HYDRUS_PLATFORM_BEGIN

#define PWMFREQ 200 // 200Hz
#define PWM_MAX_DUTY 1000000

bool pigpio_ensure_initialized()
{
    static bool done = false;
    
    if(!done)
    {
        if(pigpioInitialise() >= 0)
            done = true;
        else
            Logger::log("pigpio", "failed to init");
    }
    
    return done;
}

void pigpio_ensure_terminated()
{
    static bool done = false;
    
    if(!done)
    {
        gpioTerminate();
        done = true;
    }    
}

PWM::PWM(int channel)
{
    _ch = channel;
    
    pigpio_ensure_initialized();
}

PWM::~PWM()
{
    pigpio_ensure_terminated();
}

void PWM::set( uint32_t value )
{
    if(!pigpio_ensure_inmitialized())
        return;
    
    uint64_t dc = 1000000ull * value / (1 << PWMBITS-1);
    gpioHardwarePWM(_ch? 19 : 18, PWMFREQ, dc);
}

int PWM::bits(  ) const
{
    return PWMBITS;
}

__HYDRUS_PLATFORM_END

#else

// NOTE: ON PC - DUMMY IMPL

__HYDRUS_PLATFORM_BEGIN

PWM::PWM(int channel)
{
    _ch = channel;
}

PWM::~PWM()
{
    
}

void PWM::set( uint32_t value )
{
    (void) value;
}

int PWM::bits( ) const
{
    return PWMBITS;
}

__HYDRUS_PLATFORM_END

#endif
