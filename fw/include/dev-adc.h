
#include "platform/i2c.h"
#include "hydrus-traits.h"

__HYDRUS_BEGIN

class ADC {

    typedef Traits< ADC > Tr;    
    
public:
    
    ADC(I2CBus &b): bus(b)
    {        
    }
    
    
    int32_t get( int channel )
    {
        return channel * 100;
    }
    
    float get_alpha( int channel )
    {
        return float(get(channel)) / Tr::MAX_VALUE;
    }
    
    float get_volts( int channel )
    {
        return get(channel) * Tr::VOLTS_PER_LSB ;
    }
    
    
private:
    I2CBus &bus;
    
};

__HYDRUS_END
