#pragma once

#include "hydrus-config.h"
#include "task.h"
#include "platform/logger.h"

#include "dev-gps.h"
#include "dev-motors.h"
#include "blackboard.h"

#include "navigation/navcontroller.h"
#include "navigation/navwaypoints.h"

#include "support/util-str.h"


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
        
        
        
        if(BB->nav.rcMode)
        {
            BB->trans.begin();
            BB->nav.motor.enabled = true;
            // TODO proper trigonometry maybe
            BB->nav.motor.spdL = fmax(0, +Tr::RC_SPEED_FACTOR*BB->nav.rcVecX + fmax(0, Tr::RC_SPEED_FACTOR*BB->nav.rcVecY));
            BB->nav.motor.spdR = fmax(0, -Tr::RC_SPEED_FACTOR*BB->nav.rcVecX + fmax(0, Tr::RC_SPEED_FACTOR*BB->nav.rcVecY));
            BB->trans.end();
        }
        else
        {
            BB->trans.begin();
            if(BB->sys.state == SS_NAVIGATING)
            {
                bool cont = m_controller.step(0.04);
                if(!cont) 
                {
                    BB->nav.state = NS_NOT_NAVIGATING;
                    BB->sys.state = SS_READY;
                }
            }
            else
            {
                BB->nav.motor.enabled = false;
                BB->nav.motor.spdL = 0;
                BB->nav.motor.spdR = 0;
            }
            BB->trans.end();
        }
        
        m_motors.tick();
        
        
        if(gpsDirty)
        {
            BB->trans.begin();
            
            BB->nav.gpsHasFix = p_gpsm.fix;
            BB->nav.gpsLat = p_gpsm.lat;
            BB->nav.gpsLon = p_gpsm.lon;
            
            BB->trans.end();
            
            gpsDirty = false;
        }
        
        return true; // keep running
    }
    
    virtual uint64_t period_us() override 
    {        
        return Tr::THREAD_INTERVAL_us;
    }
    
    virtual void process_command(const char * cmd) override
    {    
        if(Util::startsWith("$NAVROUTE,", cmd))
        {
            parseRoute(cmd);
        }
        
        if(Util::startsWith("$NAVROUTE,", cmd))
        {
            beginNavigation();
        }
    }    
    
private:
    
    bool validateRoute(const Waypoints & wps)
    {
        return wps.size() > 1; // more complex logic can be put here      
    }
    
    void parseRoute(const char *cmd)
    {      
        std::string routecmd(cmd);        
        stringvec_t tok;
        Util::split(routecmd, ',', tok);
        int idx = 2;
        
        Waypoints wps;
        
        while(tok.size() - idx >= 3)
        {
            Waypoint w;
            
            w.longitude = Util::parseDouble(tok[idx]);
            w.latitude = Util::parseDouble(tok[idx+1]);
            w.acquire = tok[idx+2].size() > 0;
            w.customRadius = -1;
            
            wps.push_back(w);
            
            idx += 3;
            
            
            std::stringstream ss;
            ss << "WAYPOINT ";
            ss << w.longitude << " " << w.latitude << " ";
            ss << (w.acquire? "A" : "");
            P::Logger::log("Nav", ss.str().c_str());
            
        }
        
        if(validateRoute(wps))
        {
            m_waypoints = wps;
            P::Logger::log("nav", "Received new route");
        }
        else
        {
            m_waypoints.clear();
            P::Logger::log("nav", "Received invalid route. Current route cleared for safety.", P::Logger::ERROR);
        }
        
    }

    void beginNavigation()
    {        
        // check navigation state here
        
        if(!m_waypoints.size())
        {
            P::Logger::log("nav", "No valid route specified. Can't begin navigation.");
            return;
        }
        
        BBro->trans.begin(true);
        if(!BBro->nav.gpsHasFix)
        {
            P::Logger::log("nav", "Drone GPS has no fix. Can't begin navigation.");
            return;
        }
        BBro->trans.end(true);
        
        if(!m_controller.withinRange(m_waypoints[0].longitude, m_waypoints[0].latitude))
        {      
            P::Logger::log("nav", "Drone is not within range of base station location");
            
        }
        
        m_control.setup(m_route);
    }
    
    GPS::Message p_gpsm;
    Motors m_motors;
    
    NavController m_controller;
    Waypoints m_waypoints;
};

__HYDRUS_END
