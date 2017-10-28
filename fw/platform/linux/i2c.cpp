#include "platform/i2c.h"
#include "platform/logger.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <errno.h>

struct LinuxI2CPrivate
{
    int fd;
};

#define USE_PRIVATE LinuxI2CPrivate * const p = (LinuxI2CPrivate *)(this->_p);
#define REQUIRE_OPEN if(!p->fd) return;


__HYDRUS_PLATFORM_BEGIN

I2CBus::I2CBus( int index, Speed spd )
{
    _p = new LinuxI2CPrivate();
    
    USE_PRIVATE
    
    p->fd = 0;
    
#ifdef __arm__
    index += 1;
#endif
    
    char fname[64];
    snprintf(fname, 64, "/dev/i2c-%d", index);
    p->fd = open(fname, O_RDWR);
    
    if(p->fd < 0)
    {
        Logger::log("linux|i2c", "could not open", Logger::DEBUG);
    }
}

I2CBus::~I2CBus( )
{
    USE_PRIVATE
    REQUIRE_OPEN
    
    close(p->fd);
}

void I2CBus::select_slave( Address addr )
{
    USE_PRIVATE
    REQUIRE_OPEN
    
    if (ioctl(p->fd, I2C_SLAVE, addr) < 0) {
        Logger::log("linux|i2c", strerror(errno), Logger::ERROR);
    }
}

uint8_t I2CBus::read_byte ( Register reg )
{
    USE_PRIVATE    
    if(!p->fd) return 0xff;
    
    return i2c_smbus_read_byte_data ( p->fd, reg );
}

bool I2CBus::write_byte ( Register reg, uint8_t value )
{
    USE_PRIVATE
    if(!p->fd) return false;
    
    char buf[2];
    buf[0] = reg;
    buf[1] = value;
    
    if(::write(p->fd, buf, 2) != 2)
    {
        Logger::log("linux|i2c", "cant write", Logger::WARNING);\
        return false;
    }
    
    return true;
}

uint16_t I2CBus::read_short ( Register reg )
{
    USE_PRIVATE    
    if(!p->fd) return 0xffff;
    
    return i2c_smbus_read_word_data( p->fd, reg );
}

bool I2CBus::write_short ( Register reg, uint16_t value )
{
    USE_PRIVATE
    if(!p->fd) return false;
    
    char buf[3];
    buf[0] = reg;
    buf[1] = ((char*)&value)[1]; // high byte
    buf[2] = ((char*)&value)[0]; // low byte
    
    if(::write(p->fd, buf, 3) != 3)
        Logger::log("linux|i2c", "cant write", Logger::WARNING);
}

size_t I2CBus::read( void * dst, size_t len )
{
    USE_PRIVATE
    if(!p->fd) return -1;

    return ::read(p->fd, dst, len);    
}

size_t I2CBus::write( void * src, size_t len )
{
    USE_PRIVATE
    if(!p->fd) return -1;
    
    return ::write(p->fd, src, len);
}

__HYDRUS_PLATFORM_END
