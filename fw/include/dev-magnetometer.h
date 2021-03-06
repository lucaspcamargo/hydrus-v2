
#include "platform/i2c.h"
#include "hydrus-traits.h"

// #include <cstdio>
#include <cmath>

__HYDRUS_BEGIN


/**
 * @brief Class representing the HMC5883L magnetometer
 */
class Magnetometer
{
public:
    typedef Traits< Magnetometer > Tr;
    
    Magnetometer(P::I2CBus &b): bus(b)
    {
        bus.select_slave(Tr::I2C_ADDRESS);
        // set continuous reading mode
        bus.write_byte(0x02, 0x00);
    }
    
    ~Magnetometer()
    {
        // also nothing
    }
    
    void sample() 
    {
        bus.select_slave(Tr::I2C_ADDRESS);
        
        m_x = (read_axis(0));
        m_y = (read_axis(1));
        m_z = (read_axis(2));
        
        m_heading = atan2f( m_x, m_y );
        m_heading -= Tr::HEADING_OFFSET_RAD;
                
        while (m_heading < 0) {
            m_heading += 2 * M_PI;
        }
        
        m_heading *= 57.29577951308232f;//(180.0f / M_PI); // heading is in degrees
        
//         fprintf(stderr, "mag %f %f %f %f\n", (double)m_x, (double)m_y, (double)m_z, (double)m_heading );        
    }
        
    const float & x() const { return m_x; }
    const float & y() const { return m_y; }
    const float & z() const { return m_z; }
    const float & heading() const { return m_heading; }
    
private:
    
    inline int16_t read_axis( int axis_index )
    {
        P::I2CBus::Register reg;
        
        if(axis_index == 1) // Y
            reg = 0x07;
        else if(axis_index == 2) // z
            reg = 0x05;
        else reg = 0x03; // x, err
        
        uint16_t r = bus.read_short(reg);
        //if(r & (1 << 15))
        //    r -= 1;
        return (int16_t) r;
    }
    
    P::I2CBus &bus;
    float m_x, m_y, m_z;
    float m_heading;
    
};

__HYDRUS_END
