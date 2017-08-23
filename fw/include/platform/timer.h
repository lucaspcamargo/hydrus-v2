#pragma once

#include "hydrus-config.h"

__HYDRUS_PLATFORM_BEGIN

class Timer {
    
public:
    Timer( uint64_t period_us );
    ~Timer();
    
    /**
     * @brief wait for the next tick
     * @returns false if there was a timeout
     */
    bool wait();
    
    uint64_t period()
    {
        return m_us;
    }
    
private:
    uint64_t m_us;
    void *_p;
    int _pi;
    
};

__HYDRUS_PLATFORM_END
