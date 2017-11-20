#include "hydrus-config.h"

__HYDRUS_BEGIN

class Sensor {
    
public:
    Sensor() {}
    
    virtual float convert( float reading_volts ) = 0;
    
};


// has a single-pole IIR filter    
class PHSensor : public Sensor
{
    typedef Traits<PHSensor> Tr;
    
public:
    PHSensor() { filter_mem = -999.9f;}
    
    virtual float convert( float reading_volts ) override 
    {
        float raw;
        if(Tr::CALIBRATION_MODE)
        {
            raw = 7.0f + (reading_volts - Tr::VOLTAGE_OFFSET) * Tr::SLOPE_PH_PER_VOLT;
        }else{
            raw = reading_volts;
        }
        
            
        if( filter_mem < -100.0f )
            filter_mem = raw;
        else
            filter_mem = 0.95f*filter_mem + 0.05f*raw;
        
        return filter_mem;
        
    }
    
private:
    float filter_mem;
};


class TemperatureSensor : public Sensor
{
    typedef Traits<TemperatureSensor> Tr;
    
public:
    TemperatureSensor() {}
    
    virtual float convert( float reading_volts ) override {return (reading_volts - Tr::VOLTAGE_OFFSET) * Tr::SLOPE_CELSIUS_PER_VOLT;}
    
};


class TurbiditySensor : public Sensor
{
    typedef Traits<TurbiditySensor> Tr;
    
public:
    TurbiditySensor() {}
    
    virtual float convert( float reading_volts ) override {return Tr::formula_ntu(reading_volts);}
    
};


class BatterySensor : public Sensor
{
    typedef Traits<BatterySensor> Tr;
    
public:
    BatterySensor() {}
    
    virtual float convert( float reading_volts ) override {return reading_volts / (Tr::RESISTOR_TO_GROUND / (Tr::RESISTOR_TO_BATTERY + Tr::RESISTOR_TO_GROUND));}
    
};

__HYDRUS_END
