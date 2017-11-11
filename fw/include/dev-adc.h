#include "platform/i2c.h"
#include "platform/thread.h"
#include "hydrus-traits.h"

#include <stdio.h>

__HYDRUS_BEGIN

class ADC {

    
public:
    
    typedef Traits< ADC > Tr;    
    
    enum Channel
    {
        CH_0_PH = 0,
        CH_1_TEMP,
        CH_2_TURB,
        CH_3_BATT
    };
    
    ADC(P::I2CBus &b): bus(b)
    {
        //        osMUXpgaM-DR
        config = 0b1100001010100011;
        curr_channel = 0;
        send_config();
    }    
    
    int32_t get( int channel )
    {
        if(curr_channel != channel)
        {
            setChannel( channel );
            P::Thread::usleep( 10 * 1000 ); // sleep for 10 ms
        }
        
        int16_t ret = (int16_t) read_register(0);
        
//         fprintf(stderr, "ADC %d CH %d\n", ret, channel);
        
        return ret;
    }
    
    float get_alpha( int channel )
    {
        return float(get(channel)) / Tr::MAX_VALUE;
    }
    
    float get_volts( int channel )
    {
        return get(channel) * Tr::VOLTS_PER_LSB;
    }
    
    
private:
    
    void send_config()
    {
        
        if(!write_register( 0x01, config ))
        {
            P::Logger::log("adc", "configuration failed");
        }
    }
    
    void setChannel(int channel)
    {
        if(channel < 0 || channel > 3)
            return;
        
        config &= ~(0b11 << 12);
        config |= channel << 12;
        send_config();
        
        curr_channel = channel;
    }
    
    uint16_t read_register(uint8_t reg)
    {
        bus.select_slave(Tr::I2C_ADDRESS);
        uint16_t val = bus.read_short_smbus(reg);
        return (val >> 8) | ((val & 0xff) << 8);
    }
    
    bool write_register(uint8_t reg, uint16_t val)
    {
        bus.select_slave(Tr::I2C_ADDRESS);
        return bus.write_short_smbus(reg, (val >> 8) | ((val & 0xff) << 8));
    }
    
    P::I2CBus &bus;
    uint16_t config;
    int curr_channel;
};

__HYDRUS_END
