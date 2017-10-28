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


template<> struct Traits< I2CBus >
{
    static const uint64_t TASK_FREQ = 10;
    static const uint64_t THREAD_INTERVAL_us = 1000000ul / TASK_FREQ;
};

template<> struct Traits< ADC >
{
    static const uint32_t BITS = 16;
    static const uint32_t MAX_VALUE = (1 << BITS) - 1;
    
    static constexpr float VOLTS_PER_LSB = 0.00005035477225909819f;
};

template<> struct Traits< GPS >
{
    static const unsigned MAX_LINE_LENGTH = 160;
    static const uint64_t TASK_FREQ = 20;

    typedef float         ANGLE_DATATYPE;

    static const uint64_t THREAD_INTERVAL_us = 1000000ul / TASK_FREQ;
    
    static const bool DEBUG_PRINT_INPUT = true;
};

template<> struct Traits< Magnetometer >
{
    static const uint8_t ADDRESS = 0x1e;
    
    static constexpr float HEADING_OFFSET = 0.0f; // TODO calibrate
};

__HYDRUS_END

