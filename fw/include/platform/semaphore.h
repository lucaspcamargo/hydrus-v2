#pragma once

#include "hydrus-config.h"

__HYDRUS_PLATFORM_BEGIN

class Semaphore
{
public:
    
    Semaphore(unsigned int val = 0);
    
    ~Semaphore();
    
    bool try_p();
    
    void p();
    
    void v();
    
private:
    void *_p;
    
};

__HYDRUS_PLATFORM_END
