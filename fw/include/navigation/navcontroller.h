#include "hydrus-config.h"
#include "hydrus-traits.h"

#include "navwaypoints.h"
#include "math.h"
#include "platform/logger.h"

#include "blackboard.h"

__HYDRUS_BEGIN

#define deg2rad(x) ((x) * ((nav_f_t)0.017453292519943295))

class NavController
{
public:
    
    typedef Traits< NavController > Tr;
    typedef Tr::nav_f_t nav_f_t;
    
    
    NavController()
    {
        // nothing to initialize, all state in blackboard        
    }
    
    void setup(Waypoints &wps)
    {
        m_wps = &wps;
        m_wp = 1;
        
        BB->trans.begin();
        BB->nav.state = NS_ALIGN;
        BB->sys.state = SS_NAVIGATING;
        BB->trans.end();
    }
    
    void clearActuation()
    {
        BB->trans.begin();
        BB->nav.motor.spdL = 0;
        BB->nav.motor.spdR = 0;
        BB->nav.motor.enabled = false;
        BB->trans.end();
    }
    
    bool withinRange(nav_f_t lon, nav_f_t lat, bool exit = false)
    {
        return calcDistance_m(lon, lat, BB->nav.gpsLon, BB->nav.gpsLat) < (exit? Tr::rangeToleranceRadius_m : Tr::rangeToleranceExitRadius_m);
    }
    
    /**
     * Haversine and Forward Azimuth
     * from http://www.movable-type.co.uk/scripts/latlong.html 
     * 
     * These arguments are taken in degrees
     */
    nav_f_t calcDistance_m(nav_f_t lonA, nav_f_t latA, nav_f_t lonB, nav_f_t latB) 
    {
        const nav_f_t R = Tr::earthRadius_m; // metres
        nav_f_t lat1 = deg2rad(latA);
        nav_f_t lat2 = deg2rad(latB);
        nav_f_t delta_lat = deg2rad(latB-latA);
        nav_f_t delta_lon = deg2rad(lonB-lonA);
        
        nav_f_t a = sin(delta_lat/2) * sin(delta_lat/2) +
        cos(lat1) * cos(lat2) *
        sin(delta_lon/2) * sin(delta_lon/2);
        nav_f_t c = 2 * atan2(sqrt(a), sqrt(1-a));
        
        return R * c;
    }
    /*
     * These arguments are taken in degrees
     */
    nav_f_t calcDistanceDumb_m(nav_f_t lonA, nav_f_t latA, nav_f_t lonB, nav_f_t latB)
    {
        
//         std::stringstream ss;
//         ss << lonA << " " << latA << " ";
//         ss << lonB << " " << latB << " ";
//         Util::log("NAVCONTROL", ss.str().c_str());
        
        nav_f_t delta_lat = deg2rad(latB-latA);
        nav_f_t delta_lon = deg2rad(lonB-lonA);
        
        return Tr::earthRadius_m * (sqrt(delta_lat*delta_lat + delta_lon*delta_lon));
    }
    
    /*
     * These arguments are taken in radians
     */
    nav_f_t calcBearing_rad(nav_f_t lon1, nav_f_t lat1, nav_f_t lon2, nav_f_t lat2)
    {
        
        nav_f_t y = sin(lon2-lon1) * cos(lat2);
        nav_f_t x = cos(lat1)*sin(lat2) - sin(lat1)*cos(lat2)*cos(lon2-lon1);
        nav_f_t ret = (-atan2(y, x)) + (M_PI*0.5);
        
        if(ret < 0)
            ret += (2*M_PI);
        return ret;
    }
    
    nav_f_t fancymod(nav_f_t a, nav_f_t n)
    {
        return fmod( (fmod(a, n) + n), n );
    }
    
    nav_f_t calcAngleDifference_rad(nav_f_t sourceA, nav_f_t targetA)
    {
        nav_f_t a = targetA - sourceA;
        return fancymod((a + M_PI), 2*M_PI) - M_PI;
    }
    
    /**
     * Step navigation controller
     * @returns true if still navigating, false if not
     */
    bool step(nav_f_t time_delta)
    {
        // state timekeeping
        
        static NavigationState prev_state = (NavigationState) -1;
        static nav_f_t state_time  = 0;
        
        BBro->trans.begin(true); // BEGIN reading blackboard
        
        if(BBro->nav.state == prev_state)
        {
            state_time += time_delta;
        }
        else
        {
            state_time = 0;
            prev_state = BBro->nav.state;
        }
        
        // get gps state and position
        bool gpsFix = BBro->nav.gpsHasFix;
        nav_f_t gpsLon = BBro->nav.gpsLon, 
        gpsLat = BBro->nav.gpsLat;
        
        // get current heading
        nav_f_t magneticHeading = BBro->sensors.imuHeading;
        
        BBro->trans.end(true); // END reading blackboard
        
        // get destination angle positions
        nav_f_t destLon = ((*m_wps)[m_wp].longitude);
        nav_f_t destLat = ((*m_wps)[m_wp].latitude);
        
        // calculate distance
        nav_f_t distance = calcDistance_m(gpsLon, gpsLat, destLon, destLat);
                
        #if DRONE_COMPASS_FIXED_DECLINATION
        nav_f_t correctedHeading = magneticHeading + (DRONE_COMPASS_FIXED_DECLINATION_VALUE);
        #else
        // TODO correct using WMM
        nav_f_t correctedHeading = magneticHeading;        
        #endif
        
        nav_f_t heading = deg2rad(correctedHeading);
        nav_f_t angleToDestination = calcBearing_rad(deg2rad(gpsLon), deg2rad(gpsLat), deg2rad(destLon), deg2rad(destLat));
        
        // calculate angle difference
        nav_f_t theta = calcAngleDifference_rad(heading, angleToDestination);
        
        
        /*
         *  Now save some navigation data to the blackboard
         */
        
        BlackboardTransaction t; // BEGIN writing state to the blackboard
        
        BB->nav.geoHeading = correctedHeading;
        BB->nav.distanceFromNextWaypoint = distance;
        
        if(!gpsFix)
        {
            P::Logger::log("navcontrol", "Lost GPS fix. Skipping control, disabling motors.", P::Logger::ERROR);
            BB->nav.motor.enabled = false;
            return true;
        }
        
        switch(BB->nav.state)
        {
            case NS_NOT_NAVIGATING:
                return false; // well that was easy
                
                
            case NS_ALIGN:           // point vessel towards waypoint
            {
                // compare delta
                if(fabs(theta) < Tr::headingTolerance_rad)
                {
                    BB->nav.state = NS_ALIGN_WAIT;
                }
                else
                {
                    // angle controller
                    bool goLeft = theta > 0;
                    nav_f_t pulseTime = fmod( state_time, 2);
                    
                    if(false)//pulseTime > 1) 
                    {
                        // jolt pause time
                        BB->nav.motor.spdL = 0;
                        BB->nav.motor.spdR = 0;
                    }
                    else // give it a jolt
                    {
                        if(goLeft)
                        {
                            BB->nav.motor.spdR = Tr::speedMedium;
                            BB->nav.motor.spdL = 0;
                        }
                        else
                        {
                            BB->nav.motor.spdL = Tr::speedMedium;
                            BB->nav.motor.spdR = 0;
                        }
                    }
                    #if !DRONE_NAV_SUPRESS_MOTORS 
                    BB->nav.motor.enabled = true;
                    #endif
                }
                
                /*
                 *    std::stringstream ss;
                 *    ss << "ALIGN " << heading << " ";
                 *    ss << angleToDestination << " ";
                 *    ss << theta << " ";
                 *    // TODO write location name and distance
                 *    Util::log("NavControl", ss.str().c_str());*/
                
                return true;
            }
            
            case NS_ALIGN_WAIT:      // vessel is pointing towards waypoint
            {                
                BB->nav.motor.enabled = false;
                
                if(state_time > Tr::timeForAlign)
                {
                    BB->nav.state = NS_TRAVERSE;
                    clearActuation();
                }
                
                if(fabs(theta) > Tr::headingTolerance_rad)
                {
                    BB->nav.state = NS_ALIGN;                    
                }
                
                return true;
            }
            
            
            case NS_TRAVERSE:        // vessel moves towards waypoint
            {
                nav_f_t speed = distance < Tr::speedMediumDistance? (distance < Tr::speedLowDistance? Tr::speedLow : Tr::speedMedium) : Tr::speedFull;
                
                // update actuation
                if(2*fabs(theta) > M_PI)
                {
                    // behind me
                    BB->nav.motor.spdL = Tr::speedFull;
                    BB->nav.motor.spdR = 0;
                }
                else
                {
                    // sane thing
                    BB->nav.motor.spdL = 0.0 * speed;
                    BB->nav.motor.spdR = 0.0 * speed;
                    
                    if(theta > 0) // a bit more to the left
                        BB->nav.motor.spdR += (0.5 + 0.5*fabs(sin(theta))) * speed;
                    else          // a bit more to the right
                        BB->nav.motor.spdL += (0.5 + 0.5*fabs(sin(theta))) * speed;
                }
                
                #if !DRONE_NAV_SUPRESS_MOTORS 
                BB->nav.motor.enabled = true;
                #endif
                
                // if within radius
                if(distance < Tr::rangeToleranceRadius_m)
                {
                    BB->nav.state = NS_ARRIVAL_WAIT;
                    clearActuation();
                }
                
                
                return true;
            }
            
            case NS_ARRIVAL_WAIT:    // vessel in in radius of waypoint
                
                // if drifted to exit radius, realign, must be close
                if(distance > Tr::rangeToleranceExitRadius_m)
                {
                    BB->nav.state = NS_ALIGN;
                    return true;
                }
                
                // if waited enough time, we are in the spot
                if(state_time > Tr::timeForArrival)
                {
                    BB->nav.state = NS_ACQUIRE;
                }
                
                
            case NS_ACQUIRE:         // vessel is acquiring data
                
                if(state_time > 1) // wait a sec
                {
                    // is there another waypoint?
                    if(m_wp == (m_wps->size() - 1))
                    {
                        // end of the line
                        BB->nav.state = NS_NOT_NAVIGATING;
                        return false;
                    }
                    else
                    {
                        // advance to next waypoint
                        P::Logger::log("navcontrol", "reached point", P::Logger::INFO);
                        m_wp ++;
                        P::Logger::log("navcontrol", "going for next point", P::Logger::INFO);
                        
                        // realign
                        BB->nav.state = NS_ALIGN;
                    }
                }
                
                return true;
                
            case NS_HOMING_EMERGENCY: // vessel is returning home because something is not right   
                
                // TODO
                // set home as next point, override waypoint list
                // set mode to align
                
                return true;
                
        }
        
        // unhandled state, we halt navigation then
        return false;
    }
    
private:
    Waypoints *m_wps;
    int m_wp;
    
};

__HYDRUS_END
