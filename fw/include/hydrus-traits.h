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
    
    static constexpr float RC_SPEED_FACTOR = 0.3;
};

template<> struct Traits< CommTask >
{
    static const uint64_t TASK_FREQ = 20;
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
    
    static constexpr float HEADING_OFFSET_RAD = 0.0f;
};

template<> struct Traits< Motors >
{
    static const int P_MULT = 65536; // scale the periods from 8 bit to 24 bit
    
    // experimental values
    static const int PERIOD_MAX = 255 * P_MULT;
    static const int MIN_PERIOD = 6 * P_MULT;
    static const int MAX_PERIOD = 250 * P_MULT;
    static const int MIN_DEADBAND = 0 * P_MULT;
    
    static const bool SIM_MODE_DISABLES_MOTORS = true;
    
    static constexpr float MOTOR_FACTOR_L = 0.6;
    static constexpr float MOTOR_FACTOR_R = 0.6;
};

template<> struct Traits< PHSensor >
{
    //
    //  CALIBRATION - Blue Probe - 20/11/17
    //  Tenv = 25°C
    //  Buffer solution: pH 7.0 -- 1.82 V
    //  Buffer solution: pH 4.0 -- 1.99 V
    
    // voltage at pH = 7.0
    static constexpr float VOLTAGE_OFFSET = 1.82f; 
    
    // usually 60mV equals to -1pH from 7.0, this should be close to -16.666...
    static constexpr float SLOPE_PH_PER_VOLT = -17.64f; // 3 / (1.82 - 1.99)
    
    // this lets out the raw voltage as the pH value
    static const bool      CALIBRATION_MODE = false; 
};

template<> struct Traits< TemperatureSensor >
{
    // from TMP36 datasheet
    static constexpr float VOLTAGE_OFFSET = 0.5f;
    static constexpr float SLOPE_CELSIUS_PER_VOLT = 100.0f;
};

template<> struct Traits< TurbiditySensor >
{
    // linear regression from datasheet graph
    // a  261.0545
    // b -2607.58
    // c  6366.978
    // y = axx + bx + c
    
    inline static float formula_ntu( float volts ) { return 261.0545f * volts * volts - 2607.58f * volts + 6366.978f; };
};



template<> struct Traits< BatterySensor >
{
    static constexpr float RESISTOR_TO_BATTERY = 47e3f; // 47 kOhm
    static constexpr float RESISTOR_TO_GROUND  = 6.8e3f;  // 6.8 kOhm
};

template<> struct Traits< NavController >
{
    typedef float nav_f_t;
    
    static constexpr nav_f_t earthRadius_m = 6371000;
    
    static constexpr nav_f_t rangeToleranceRadius_m = 7;
    static constexpr nav_f_t rangeToleranceExitRadius_m = 12;
    static constexpr nav_f_t headingTolerance_rad = 10 * 0.017453292519943295f; // 10 degrees in radians
    
    static constexpr nav_f_t SPEED_FACTOR = 0.3;
    
    static constexpr nav_f_t speedFull = (0.5 * SPEED_FACTOR);
    static constexpr nav_f_t speedMedium = (0.25 * SPEED_FACTOR);
    static constexpr nav_f_t speedLow = (0.1 * SPEED_FACTOR);
    
    static constexpr nav_f_t speedMediumDistance = 20; //meters
    static constexpr nav_f_t speedLowDistance = 5; //meters
    
    static constexpr nav_f_t timeForAlign = 3; // 3 seconds
    static constexpr nav_f_t timeForArrival = 5; // 5 seconds
    
    static const bool        USE_FIXED_DECLINATION = true;
    static constexpr nav_f_t FIXED_DECLINATION_DEG = -19.5666666666f;
};


template<> struct Traits< Station >
{
#ifdef __arm__
    static constexpr const char * STATION_HOST = "10.42.0.1";    
#else
    static constexpr const char * STATION_HOST = "127.0.0.1";
#endif
    static const int CONNECTION_ATTEMPT_PERIOD_MS = 5000;
    static const int STATION_PORT = 6666;
    static const int STATION_BB_SEND_INTERVAL = 50;
};
__HYDRUS_END

