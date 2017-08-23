#include "platform/timer.h"

// this linux implementation uses timer_fd
#include <sys/timerfd.h>
#include <unistd.h>

#define MILLION 1000000ul

void set_timespec_us( uint64_t us, timespec &ts )
{
    ts.tv_sec = us / MILLION;
    ts.tv_nsec = (us % MILLION) * 1000ul; 
}


__HYDRUS_PLATFORM_BEGIN

Timer::Timer( uint64_t period_us ) : m_us( period_us )
{
    _pi = timerfd_create(CLOCK_MONOTONIC, 0);
    
    struct timespec ts;
    set_timespec_us(m_us, ts);
    
    struct itimerspec its;
    its.it_value = ts;
    its.it_interval = ts;
    
    timerfd_settime(_pi, 0, &its, 0);
}

Timer::~Timer()
{
    close( _pi );
}

bool Hydrus::P::Timer::wait()
{
    uint64_t expiries;
    
    read(_pi, &expiries, sizeof(uint64_t)); // block until expiration happened
    return expiries <= 1; // false if timeout
}


__HYDRUS_PLATFORM_END
