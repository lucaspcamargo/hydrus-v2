#pragma once

#include "hydrus-config.h"

#include "task.h"
#include "platform/logger.h"

__HYDRUS_BEGIN

class DummyTask: public Task
{
public:
    uint64_t period_us() override
    {
        return 500000ul;
    }
    
    bool tick() override
    {
        static int i = 0;
        P::Logger::log("dummytask", "Hello World");                
        return (++i) < 5;
    }
    
};

__HYDRUS_END

