#pragma once

#include "hydrus-config.h"
#include "hydrus-traits.h"

#include "task.h"
#include "platform/logger.h"

#include "support/station.h"
#include "support/util-str.h"

__HYDRUS_BEGIN

class CommTask : public Task, public P::Logger::LogListener
{
public:
    
    typedef Traits< CommTask > Tr;
    
    CommTask(Task &navTask) : Task(), m_nav_task(navTask)
    {
        P::Logger::register_listener(this);
    }        
    
    virtual bool tick() override 
    {
        m_st.tick();
        
        if(BB->comm.connected != m_st.connected())
        {
            BB->trans.begin();
            BB->comm.connected = m_st.connected();
            BB->trans.end();
        }
        
        std::string msg;
        while(m_st.hasMessages())
        {
            msg = m_st.unqueueMessage();
            
            route_command(msg);
        }
        
        return true; // keep running
    }
    
    virtual uint64_t period_us() override 
    {        
        return Tr::THREAD_INTERVAL_us;
    }
    
    virtual void log_received(const char * o, const char * what, P::Logger::Level lvl) override
    {
        std::string logline;
        if(o)
        {
            logline.append("[");
            logline.append(o);
            logline.append("] ");
        }
        logline.append(P::Logger::log_severity_str(lvl));
        logline.append(": ");
        logline.append(what);
        
        m_st.queueLog(logline);
    }
    
private:
    
    void route_command(const std::string & msg)
    {
        P::Logger::log("comm|recv", msg.c_str());
        
        
        // do not route for now, just process the messages
        if(!msg.find("$HALT"))
        {
            BB->trans.begin();
            BB->sys.state = SS_SHUTDOWN;
            BB->trans.end();
        }    
        if(!msg.find("$REBOOT"))
        {
            BB->trans.begin();
            BB->sys.reboot = true;
            BB->sys.state = SS_SHUTDOWN;
            BB->trans.end();
        }
        
        if(!msg.find("$RCON"))
        {
            P::Logger::log("Comm", "Turned on RC");
            BB->trans.begin();
            BB->nav.rcVecX = BB->nav.rcVecX = 0;
            BB->nav.rcMode = true;
            BB->trans.end();
        }
        
        if(!msg.find("$RCOFF"))
        {
            P::Logger::log("Comm", "Turned off RC");
            BB->trans.begin();
            BB->nav.motor.spdL = BB->nav.motor.spdR = 0;
            BB->nav.rcMode = false;
            BB->trans.end();
        }
        
        if(!msg.find("$RCUP,"))
        {
            
            stringvec_t tok;
            Util::split(msg, ',', tok);
            
            BB->trans.begin();
            if(tok.size() == 3)
            {
                BB->nav.rcVecX = Util::parseFloat(tok[1]);
                BB->nav.rcVecY = Util::parseFloat(tok[2]);
            }
            else
            { 
                BB->nav.rcVecX = 0;
                BB->nav.rcVecY = 0;
            }
            BB->trans.end();
        }
        
        if(!msg.find("$NAV"))
        {
            m_nav_task.process_command(msg.c_str());
        }
        
        // TODO restore rounting of these commands
        
        if(!msg.find("$SIM"))
        {
//             if(!m_sim)
//                 m_sim = new SimulationSlave();
//             
//             m_sim->onCommandReceived(msg);
        }
        
        if(!msg.find("$GPS,"))
        {
//             Serial1.print(msg.c_str() + 5);
//             Serial1.print("\r\n");
        }
        
    }
    
    Station m_st;
    Task &m_nav_task;
};

__HYDRUS_END



