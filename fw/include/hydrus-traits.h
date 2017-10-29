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
    
    static constexpr float MAX_BATT_V = 16.0f;
    static constexpr float MIN_BATT_V = 12.5f;
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
    static const uint8_t I2C_ADDRESS = 0x48; // 72
    
    static const uint32_t BITS = 16;
    static const uint32_t MAX_VALUE = (1 << BITS) - 1;
    
    static constexpr float VOLTS_PER_LSB = 0.000125f;  // 125uV
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
    static const uint8_t I2C_ADDRESS = 0x1e;
    
    static constexpr float HEADING_OFFSET = 0.0f; // TODO calibrate
};

__HYDRUS_END

