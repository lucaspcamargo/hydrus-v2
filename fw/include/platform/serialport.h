#pragma once

#include "hydrus-config.h"

__HYDRUS_PLATFORM_BEGIN

struct SerialPortPrivate;

class SerialPort
{           
public: 
    
    /** The common API does not have a default speed */
    static const int SPEED_DEFAULT = 1; 
    
    SerialPort(int index, int speed = SPEED_DEFAULT, int bits = 8, int parity = 0, int stop = 1);
    
    ssize_t read( void* dst, size_t count );
    ssize_t write( void *src, size_t count );
    ssize_t available();
    
    bool ready();
    bool can_read_line();
    
    static int get_index( const char *id );
    
private:
    SerialPortPrivate *_p;
};

__HYDRUS_PLATFORM_END
