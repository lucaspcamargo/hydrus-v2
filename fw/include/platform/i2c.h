/** @file i2c.h
 *  @brief an i²c bus
 */

#pragma once

#include "hydrus-config.h"

__HYDRUS_PLATFORM_BEGIN

class I2CBus {
public:
    enum Speed {
        LowSpeed,
        MediumSpeed,
        HighSpeed,
        DefaultSpeed = MediumSpeed,
    };
    
    typedef uint8_t Address;
    typedef uint8_t Register;
    
    I2CBus( int index, Speed spd );
    ~I2CBus( );
    
    void select_slave( Address dev ) ;
    
    uint8_t read_byte ( Register reg ); 
    bool write_byte ( Register reg, uint8_t value );
    
    uint16_t read_short ( Register reg ); 
    bool write_short ( Register reg, uint16_t value );
    
    size_t read( void * dst, size_t len );
    size_t write( void * src, size_t len );
        
    static I2CBus & getBusInstance( int index );
    
private:
    void *_p;
};

__HYDRUS_PLATFORM_END
