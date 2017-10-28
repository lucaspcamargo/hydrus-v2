
#include "platform/i2c.h"
#include "hydrus-traits.h"

__HYDRUS_BEGIN


/**
 * @brief Class representing the HMC5883L magnetometer
 */
class Magnetometer
{
public:
    typedef Traits< Magnetometer > Tr;
    
    Magnetometer(I2CBus &b): bus(b)
    {
        // do nothing
    }
    
    ~Magnetometer()
    {
        // also nothing
    }
    
    void sample() 
    {
        
        
    }
        
    static const float & x() const { return m_x; }
    static const float & y() const { return m_y; }
    static const float & z() const { return m_z; }
    static const float & heading() const { return m_heading; }
    
private:
    I2CBus &bus;
    float m_x, m_y, m_z;
    float m_heading;
    
};

__HYDRUS_END
