#pragma once

#include "hydrus-config.h"

__HYDRUS_BEGIN

class Task
{
public:
    virtual bool tick() = 0;
    virtual uint64_t period_us() = 0;
};

__HYDRUS_END