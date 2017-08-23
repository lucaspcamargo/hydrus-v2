#pragma once

#include "hydrus-config.h"

#include "platform/thread.h"
#include "platform/timer.h"
#include "platform/logger.h"
#include "task.h"

__HYDRUS_BEGIN

class PeriodicThread : public P::Thread {
public:
    PeriodicThread( Task *task ) : 
    P::Thread( P::Thread::NORMAL ), 
    m_task(task),
    m_timer(task->period_us())
    {
        m_running = true;
    }
    
    int main() override 
    {        
        while(m_running)
        {
            m_timer.wait();
            m_running = m_task->tick();
        }
     
        return 0;
    }
    
    void stop() { m_running = false; }
    
private:
    Task *m_task;
    P::Timer m_timer;
    volatile bool m_running;
};

__HYDRUS_END
