
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
        config = 0b1100001010000011;
        curr_channel = 0;
        sendConfig();
    }    
    
    int32_t get( int channel )
    {
        if(curr_channel != channel)
        {
            setChannel( channel );
            P::Thread::usleep( 10 * 1000 ); // sleep for 10 ms
        }
        
        int32_t ret = readRegister(0);
        
        return ret;
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
    
    void sendConfig()
    {
        writeRegister( 0x01, config );
    }
    
    void setChannel(int channel)
    {
        if(channel < 0 || channel > 3)
            return;
        
        config &= 0b1100111111111111;
        config |= channel << 12;
        sendConfig();
        
        curr_channel = channel;
    }
    
    uint16_t readRegister(uint8_t reg)
    {
        bus.select_slave(Tr::I2C_ADDRESS);
        bus.write(&reg, 1);
        char readout[2];
        bus.read(readout, 2);
        return readout[0] << 8 | readout[1];
    }
    
    void writeRegister(uint8_t reg, uint16_t value)
    {
        bus.select_slave(Tr::I2C_ADDRESS);
        bus.write_short(reg, value);
    }
    
    P::I2CBus &bus;
    uint16_t config;
    int curr_channel;
};

__HYDRUS_END
