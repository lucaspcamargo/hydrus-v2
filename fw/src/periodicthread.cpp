#include "periodicthread.h"

__HYDRUS_BEGIN

    int PeriodicThread::main()
    {                
        while(m_running)
        {
            m_timer.wait();
            m_running = m_task->tick();
        }
     
        return 0;
    }
    
__HYDRUS_END
