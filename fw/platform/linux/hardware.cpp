#include <hydrus-config.h>

#include "platform/gpio.h"
#include "platform/serialport.h"
#include "platform/logger.h"

#include <sys/types.h>
#include <sys/stat.h>       
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/termios.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>


/* GPIO tables resolve a "platform" gpio number into a system-specific gpio number */
/* -1 means the pin is invalid  */
#define GPIO_TBL GPIO_DESKTOP_TBL

static const int8_t GPIO_DESKTOP_TBL[64] = {
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
};

static const int8_t GPIO_RBPI_TBL[64] = {
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
    -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1, 
};


__HYDRUS_PLATFORM_BEGIN



// GPIO

struct GPIOPrivate
{
    int pin;
    GPIO::Direction dir;
    GPIO::Flags flags;
    
    GPIOPrivate( int syspin, GPIO::Direction direction, GPIO::Flags f)
    {
        pin = syspin;
        dir = direction;
        flags = f;
    }
};

GPIO::GPIO( unsigned int platformpin, Direction direction, Flags f )
{
    int8_t syspin = GPIO_TBL[platformpin];
    
    if( syspin < 0 )
    {
        _p = 0;
        P::Logger::log("gpio", "platform gpio has no system equivalent");
        return;
    }
    
    _p = new GPIOPrivate( syspin, direction, f );
    
    int expfd = open("/sys/class/gpio/export", O_WRONLY);
    if (-1 == expfd) 
    {    
        P::Logger::log("gpio", "failed to export gpio");
        _p = 0;
        return;
    }
    
    char expbuf[4];
    int expcount = snprintf(expbuf, 4, "%d", syspin);
    if(expcount != write(expfd, expbuf, expcount))
    {
        P::Logger::log("gpio", "failed to export gpio?");        
    }
    close( expfd );
    
    configure( direction, f );    
}

bool GPIO::configure( Direction d, Flags f )
{
    if(!_p)
        return false;
    
    if((f & PULLDOWN)||(f & PULLUP)||(f & OPEN_COLLECTOR))
        P::Logger::log("gpio", "requested features unsupported on linux platform");
    
    GPIOPrivate *p = (GPIOPrivate *)_p;
    char dirfile[256];
    
    snprintf(dirfile, sizeof(dirfile), "/sys/class/gpio/gpio%d/direction", p->pin);
    int dirfd = open(dirfile, O_WRONLY);
    
    if(!dirfd)
    {
        P::Logger::log("gpio", "failed to set direction");
        return false;
    }
    
    const char *dirstr;
    if( d == OUTPUT )
    {
        if(f & INIT_OUTPUT_AS_HIGH)
            dirstr = "high";
        else
            dirstr = "low";
    }
    else
        dirstr = "in";
    
    write( dirfd, dirstr, strlen(dirstr) );
    close( dirfd );
        
    return true;
    
}


GPIO::Level GPIO::get()
{
    if(!_p)
        return LOW;
    
    GPIOPrivate *p = ((GPIOPrivate*)_p);
    
    char dirfile[256];
    snprintf(dirfile, sizeof(dirfile), "/sys/class/gpio/gpio%d/value", p->pin);
    int valfd = open(dirfile, O_RDONLY);
    
    if(!valfd){
        P::Logger::log("gpio", "failed to open value fd");
        return INVALID;
    }
    
    char c;
    if (read(valfd, &c, 1) != 1)
    {
        P::Logger::log("gpio", "could not read value");        
        return INVALID;
    }
    
    close(valfd);
    
    GPIO::Level ret = INVALID;
    
    if(c == '1') ret = HIGH;
    if(c == '0') ret = LOW;    
    
    return ret;
}

void GPIO::set( Level l )
{
    if(!_p)
        return;
    
    GPIOPrivate *p = ((GPIOPrivate*)_p);
    
    char dirfile[256];
    snprintf(dirfile, sizeof(dirfile), "/sys/class/gpio/gpio%d/value", p->pin);
    int valfd = open(dirfile, O_WRONLY);
    
    if(!valfd){
        P::Logger::log("gpio", "failed to open value fd");
        return;
    }
    
    char c = (l == HIGH? '1':'0');
    
    if (write(valfd, &c, 1) != 1)
    {
        P::Logger::log("gpio", "could not set value");     
        return;
    }
    
    close(valfd);
}


// Serial Port

struct SerialPortPrivate {
    int fd;
    int index;
    int speed;
    int bits;
    int parity;    
    
    const char *index_to_dev_path( int index )
    {
        if( index == 0 )
        {
            return "/dev/ttyAMA0";
        }
        else if( index == 1 )
        {
            return "/dev/ttyUSB1";
        }
        else if( index == 2 )
        {
            return "/dev/ttyUSB2";
        }
        else if( index == 3 )
        {
            return "/dev/ttyUSB3";
        }
        return 0;
    }
    
    speed_t speed_constant( int speed )
    {
        switch( speed )
        {
            case 4800:
                return B4800;
                
            case 9600:
                return B9600;
                
            case 19200:
                return B19200;
                
            case 115200:
                return B115200;
                
            case 230400:
                return B230400;
                
            case 460800:
                return B460800;
                
            case 576000:
                return B576000;
                
            case 921600:
                return B921600;
                
            case 1000000:
                return B1000000;
        }
        
        return B9600; // the universal default (?)
    }
    
    void init()
    {
        const char * path = index_to_dev_path(index);
        if(!path)
            return;
        
        errno = 0;
        
        fd = open(path, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
                
        if(!fd || errno)
        {
            Logger::log("linux|serial", "could not open");
        }
        
        struct termios opt;
        tcgetattr(fd, &opt);
        
        speed_t spdCfg = speed_constant(speed);

        cfsetispeed(&opt, spdCfg);
        cfsetospeed(&opt, spdCfg);
        
        opt.c_cflag |= (CLOCAL | CREAD);
        
        tcsetattr(fd, TCSAFLUSH, &opt);
    }
};

SerialPort::SerialPort(int index, int speed, int bits, int parity, int stop)
{
    _p = new SerialPortPrivate();
    _p->fd = 0;
    _p->index = index;
    _p->speed = speed;
    _p->bits = bits;
    _p->parity = parity;

    _p->init();
}

bool SerialPort::ready()
{
    return _p->fd;
}
    
ssize_t SerialPort::read( void* dst, size_t count )
{
    if(!ready())
        return -1;
    
    return ::read(_p->fd, dst, count); 
}

ssize_t SerialPort::write( void *src, size_t count )
{
    if(!ready())
        return -1;
    
    return ::write(_p->fd, src, count );
}

ssize_t SerialPort::available()
{
    if(!ready())
        return -1;
    
    int ret = -1;
    ioctl(_p->fd, FIONREAD, &ret);
    return ret>=0? ret : 0;
}

// this is static
int SerialPort::get_index( const char *id )
{
//     std::string device(id);
//     
//     if( device.starts_with("ttyS") )
//     {
//         
//     }
//     
    return id[strlen(id)-1] - '0'; //TODO STUB
}
    
__HYDRUS_PLATFORM_END
