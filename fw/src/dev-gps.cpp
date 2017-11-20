#include <dev-gps.h>
#include <platform/mutex.h>
#include <platform/thread.h>
#include <platform/logger.h>
#include <platform/serialport.h>

#include "periodicthread.h"
#include "nmea.h"
#include "nmea/gprmc.h"

__HYDRUS_BEGIN

struct GPSPrivate_st {
    
    GPS::Message m;
    bool dirty;
    
//     P::FunctionThread<GPS> *thread;
    PeriodicThread *thread;
    P::Mutex *lock;
    
    P::SerialPort *serial;
    char linebuf[GPS::Tr::MAX_LINE_LENGTH];
    int inbuf;
    
} _p;


void GPS::init()
{
    P::Logger::log("gps", "initializing");
    bzero(&_p, sizeof(_p));    
    
    _p.lock = new P::Mutex();    
    _p.serial = new P::SerialPort( 0, 9600, 8, 0, 1 );
    
    class GPSTask : public Task {
    public:    
        virtual bool tick() override { return GPS::tick(); };
        virtual uint64_t period_us() override { return GPS::Tr::THREAD_INTERVAL_us; };    
    };
    
    static GPSTask t;
    _p.thread = new PeriodicThread(&t);
    _p.thread->start();
}

const GPS::Message GPS::get_message_copy()
{
    P::MutexLock(_p.lock);
    _p.dirty = false;
    return _p.m;
}

const GPS::Message & GPS::lock_message()
{
    _p.lock->lock();
    return _p.m;
}

void GPS::unlock_message()
{
    _p.lock->unlock();
}

bool GPS::has_message()
{
    P::MutexLock(_p.lock);
    return _p.dirty;
}

void GPS::received_message(Message &msg)
{
    _p.lock->lock();
    
    _p.m = msg;
    _p.dirty = true;
    
    _p.lock->unlock();
}

void _gps_lineReceived()
{
    if(GPS::Tr::DEBUG_PRINT_INPUT) 
        P::Logger::log("gps", _p.linebuf, P::Logger::DEBUG);
    
    nmea_s *data;
    
    data = nmea_parse(_p.linebuf, strlen(_p.linebuf), 0);
    
    if (data && NMEA_GPRMC == data->type) {
        GPS::Message msg;
        
        nmea_gprmc_s *gprmc = (nmea_gprmc_s *) data;
        msg.unixtime = mktime(&gprmc->time);
        
        if(gprmc->latitude.cardinal == NMEA_CARDINAL_DIR_UNKNOWN || gprmc->longitude.cardinal == NMEA_CARDINAL_DIR_UNKNOWN)
        {
            msg.fix = false;
        }
        else
        {
            msg.fix = true;
            
            msg.lat = gprmc->latitude.degrees + gprmc->latitude.minutes/60.0;
            if(gprmc->latitude.cardinal == NMEA_CARDINAL_DIR_SOUTH)
                msg.lat = -msg.lat;
            
            msg.lon = gprmc->longitude.degrees + gprmc->longitude.minutes/60.0;
            if(gprmc->longitude.cardinal == NMEA_CARDINAL_DIR_WEST)
                msg.lon = -msg.lon;
        }
        
        
        GPS::received_message(msg);
    }
    
    if(data)
        nmea_free(data);
}

bool GPS::tick()
{
    if(!_p.serial->ready())
        return true;
    
    while(_p.serial->available())
    {
        // read bytes till line reached
        char byte;
        _p.serial->read(&byte, 1);
        
        if(byte == '\n')
        {
            _p.linebuf[_p.inbuf] =  '\0';
            if(_p.inbuf)
                _gps_lineReceived();
            _p.inbuf = 0;
        }
        else if(_p.inbuf < (sizeof(_p.linebuf)-1))
        {
            _p.linebuf[_p.inbuf] = byte;
            _p.inbuf ++;
        }
        else
        {
            P::Logger::log("gps", "line too long", P::Logger::WARNING);
            _p.inbuf = 0;
            while(_p.serial->available() && byte != '\n')
                _p.serial->read(&byte, 1); // empty buffer
            P::Logger::log("gps", "cleared buffer", P::Logger::WARNING);
        }
    }
    
    return true;
}


__HYDRUS_END
