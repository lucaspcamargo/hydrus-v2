#pragma once

#include "hydrus-config.h"
#include "hydrus-traits.h"

#include "task.h"
#include "platform/logger.h"
#include "support/sys-stats.h"

__HYDRUS_BEGIN

class SystemTask : public Task
{
public:
    
    typedef Traits< SystemTask > Tr;
    
    SystemTask() : Task()
    {
        BB->trans.begin();        
        BB->sys.state = SS_READY;        
        BB->trans.end();
    }        
    
    virtual bool tick() override 
    {
        static int counter = 0;
        
        counter ++;
        if(counter == Tr::TASK_FREQ) // every second
        {
            counter = 0;
            update_system_stats();            
        }
        
        BB->trans.begin();        
        if(BB->sys.state == SS_SHUTDOWN)
        {
            // TODO sync code goes here
            BB->sys.state = SS_HALTED;        
        }
        BB->trans.end();
        
        BBro->trans.begin(true); // read-only interaction
        if(BBro->sys.state == SS_HALTED)
        { 
            if(BBro->sys.reboot)
                system("reboot"); // reboot linux
            else
                system("shutdown now"); // clean up linux
        }
        BBro->trans.end(true);
        
        return true; // keep running
    }
    
    virtual uint64_t period_us() override 
    {        
        return Tr::THREAD_INTERVAL_us;
    }
        
    
private:
    
    void update_system_stats()
    {
        BB->trans.begin();        
        BB->sys.coreTemp = readSOCTemperature();   
        BB->sys.cpuLoad = readCPULoad();
        BB->trans.end();        
    }    
};

__HYDRUS_END


