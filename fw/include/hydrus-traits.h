#pragma once

#include <hydrus-types.h>

__HYDRUS_BEGIN

template < typename T > struct Traits
{

};


template<> struct Traits< SystemTask >
{
    static const uint64_t TASK_FREQ = 2;
    static const uint64_t THREAD_INTERVAL_us = 1000000ul / TASK_FREQ;
};

template<> struct Traits< SensingTask >
{
    static const uint64_t TASK_FREQ = 30;
    static const uint64_t THREAD_INTERVAL_us = 1000000ul / TASK_FREQ;
};

template<> struct Traits< NavigationTask >
{
    static const uint64_t TASK_FREQ = 10;
    static const uint64_t THREAD_INTERVAL_us = 1000000ul / TASK_FREQ;
};

template<> struct Traits< GPS >
{
    static const unsigned MAX_LINE_LENGTH = 160;
    static const uint64_t TASK_FREQ = 20;

    typedef double ANGLE_DATATYPE;

    static const uint64_t THREAD_INTERVAL_us = 1000000ul / TASK_FREQ;
};

__HYDRUS_END

