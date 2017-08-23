#pragma once

#include "hydrus-config.h"

__HYDRUS_PLATFORM_BEGIN

class Mutex
{
public:
    Mutex(); 
    ~Mutex();
    bool tryLock();
    void lock();
    void unlock();
    
private:
    void *_p;
};

class MutexLock
{
public:
    MutexLock(Mutex & m):_m(m){ _m.lock(); }
    ~MutexLock(){ _m.unlock(); }
    
private:
    Mutex &_m;
};

__HYDRUS_PLATFORM_END
