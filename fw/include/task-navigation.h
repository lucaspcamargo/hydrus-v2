#pragma once

#include "hydrus-config.h"
#include "task.h"
#include "platform/logger.h"

#include "dev-gps.h"
#include "blackboard.h"

#include "navigation/navcontroller.h"
#include "navigation/navwaypoints.h"

__HYDRUS_BEGIN

class NavigationTask : public Task
{
public:
    
    typedef Traits< NavigationTask > Tr;
    
    NavigationTask() : Task()
    {
        GPS::init();    
        bzero(&p_gpsm, sizeof(p_gpsm));
    }
        
    
    virtual bool tick() override 
    {        
        bool gpsDirty = false;
        if(GPS::has_message())
        {
            p_gpsm = GPS::get_message_copy();
            gpsDirty = true;
        }
        
        if(gpsDirty)
        {
            BB->trans.begin();
            
            BB->nav.gpsHasFix = p_gpsm.fix;
            BB->nav.gpsLat = p_gpsm.lat;
            BB->nav.gpsLon = p_gpsm.lon;
            
            BB->trans.end();
        }
        
        return true; // keep running
    }
    
    virtual uint64_t period_us() override 
    {        
        return Tr::THREAD_INTERVAL_us;
    }
    
    
private:
    GPS::Message p_gpsm;
    
    NavController m_controller;
    Waypoints m_waypoints;
};

__HYDRUS_END
