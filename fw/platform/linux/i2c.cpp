#include "platform/i2c.h"
#include "platform/logger.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <errno.h>

#define USE_PRIVATE LinuxI2CPrivate * const p = (LinuxI2CPrivate *)(this->_p);
#define REQUIRE_OPEN if(!p->fd) return;

#ifdef __arm__

__HYDRUS_PLATFORM_BEGIN

#include <pigpio.h>
#include "pigpio/pigpio-helper.h"

struct LinuxI2CPrivate
{
    int fd;
    I2CBus::Address lastAddr;
};

I2CBus::I2CBus( int index, Speed spd )
{
    _p = new LinuxI2CPrivate();
    
    USE_PRIVATE
    
    p->fd = 0;
    index += 1;
    
    pigpio_ensure_initialized();
}

I2CBus::~I2CBus( )
{
    USE_PRIVATE
    REQUIRE_OPEN
    
    select_slave(0);
    
    pigpio_ensure_terminated();
}

void I2CBus::select_slave( Address addr )
{
    USE_PRIVATE
    REQUIRE_OPEN
    
    if(p->lastAddr == addr)
        return;
    
    if(p->fd)
    {
        i2cClose(p->fd);
        p->fd = 0;
    }
    
    if(addr)
        p->fd = i2cOpen(1, addr, 0);
    
    p->lastAddr = addr;
}

uint8_t I2CBus::read_byte ( Register reg )
{
    USE_PRIVATE    
    if(!p->fd) return 0xff;
    
    return i2cReadByteData(p->fd, reg);
}

bool I2CBus::write_byte ( Register reg, uint8_t value )
{
    USE_PRIVATE
    if(!p->fd) return false;
    
    return !i2cWriteByteData(p->fd, reg, value);
}

uint16_t I2CBus::read_short ( Register reg, bool lendian )
{
    USE_PRIVATE    
    if(!p->fd) return 0xffff;
    
    return i2cReadWordData(p->fd, reg);
}

bool I2CBus::write_short ( Register reg, uint16_t value )
{
    USE_PRIVATE
    if(!p->fd) return false;
    
    return !i2cWriteWordData(p->fd, reg, value);
}


uint16_t I2CBus::read_short_smbus ( Register reg)
{
    USE_PRIVATE    
    if(!p->fd) return 0xffff;
    
    return i2cReadWordData(p->fd, reg);
}

bool I2CBus::write_short_smbus ( Register reg, uint16_t value )
{
    USE_PRIVATE
    if(!p->fd) return false;
    
    return !i2cWriteWordData(p->fd, reg, value);
}

size_t I2CBus::read( void * dst, size_t len )
{
//     USE_PRIVATE    
//     if(!p->fd) return 0xff;
//     
//     return i2cReadByteData(p->fd, reg);
    return -1;
}

size_t I2CBus::write( void * src, size_t len )
{
//     USE_PRIVATE
//     if(!p->fd) return -1;
//     
//     return ::write(p->fd, src, len);
    return -1;
}

I2CBus & I2CBus::getBusInstance( int index )
{
    (void) index;
    
    static I2CBus * bus = 0;
    
    if (!bus)
        bus = new I2CBus(index, I2CBus::DefaultSpeed);
    
    return *bus;
}

__HYDRUS_PLATFORM_END


#else


__HYDRUS_PLATFORM_BEGIN

struct LinuxI2CPrivate
{
    int fd;
    I2CBus::Address lastAddr;
};

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
    
    p->fd = 0;
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
    
    p->lastAddr = addr;
}

uint8_t I2CBus::read_byte ( Register reg )
{
    USE_PRIVATE    
    if(!p->fd) return 0xff;
    
    i2c_rdwr_ioctl_data opdata;    
    i2c_msg msgs[2];
    
    opdata.msgs = msgs;
    opdata.nmsgs = 2;
    
    char readbuf[1];
    readbuf[0] = 0xff;
    
    msgs[0].addr = p->lastAddr;
    msgs[0].flags = 0;
    msgs[0].buf = (char*)&reg;
    msgs[0].len = 1;
    
    msgs[1].addr = p->lastAddr;
    msgs[1].flags = I2C_M_RD;
    msgs[1].buf = readbuf;
    msgs[1].len = 1;
    
    ioctl( p->fd, I2C_RDWR, opdata );
    
    return static_cast<uint8_t>(readbuf[0]);
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

uint16_t I2CBus::read_short ( Register reg, bool lendian )
{
    USE_PRIVATE    
    if(!p->fd) return 0xffff;
    
    //return i2c_smbus_read_word_data(p->fd, reg);
    
    i2c_rdwr_ioctl_data opdata;    
    i2c_msg msgs[2];
    
    opdata.msgs = msgs;
    opdata.nmsgs = 2;
    
    char readbuf[2];
    readbuf[0] = 0xf8;
    readbuf[1] = 0xf7;
    
    msgs[0].addr = p->lastAddr;
    msgs[0].flags = 0;
    msgs[0].buf = (char*)&reg;
    msgs[0].len = 1;
    
    msgs[1].addr = p->lastAddr;
    msgs[1].flags = I2C_M_RD;
    msgs[1].buf = readbuf;
    msgs[1].len = 2;
    
    int ret = ioctl( p->fd, I2C_RDWR, &opdata );    
    if(ret < 2)
        fprintf(stderr, "READ_SHORT IOCTL ERROR %d\n", ret);
    
    return lendian? readbuf[0] | (readbuf[1] << 8) : readbuf[1] | (readbuf[0] << 8);
}

bool I2CBus::write_short ( Register reg, uint16_t value )
{
    USE_PRIVATE
    if(!p->fd) return false;
    
    char buf[3];
    buf[0] = reg;
    buf[1] = 0xff & (value >> 8); // high byte
    buf[2] = 0xff & value; // low byte
    
    if(::write(p->fd, buf, 3) != 3)
        Logger::log("linux|i2c", "cant write", Logger::WARNING);
}


uint16_t I2CBus::read_short_smbus ( Register reg)
{
    USE_PRIVATE
    return i2c_smbus_read_word_data(p->fd, reg);
}

bool I2CBus::write_short_smbus ( Register reg, uint16_t value )
{
    USE_PRIVATE
    return !i2c_smbus_write_word_data(p->fd, reg, value);
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

I2CBus & I2CBus::getBusInstance( int index )
{
    (void) index;
    
    static I2CBus * bus = 0;
    
    if (!bus)
        bus = new I2CBus(index, I2CBus::DefaultSpeed);
    
    return *bus;
}

__HYDRUS_PLATFORM_END


#endif
