#include "platform/gpio.h"
#include "platform/serialport.h"

#include <sys/types.h>
#include <sys/stat.h>       
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/termios.h>

#include <string.h>

__HYDRUS_PLATFORM_BEGIN

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
            return "/dev/ttyS0";
        }
        else if( index == 1 )
        {
            return "/dev/ttyS1";
        }
        else if( index == 2 )
        {
            return "/dev/ttyS2";
        }
        else if( index == 3 )
        {
            return "/dev/ttyS3";
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
        
        fd = open(path, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
        
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
    return ret;
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
