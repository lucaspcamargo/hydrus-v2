#include <errno.h>
#include <stdio.h>
#include "platform/thread.h"
#include "platform/semaphore.h"
#include "platform/mutex.h"
#include "platform/logger.h"

#include "pthread.h"
#include "semaphore.h"
#include "sched.h"

__HYDRUS_PLATFORM_BEGIN


/// SEMAPHORE

Semaphore::Semaphore(unsigned int val)
{
    _p = new sem_t;
    sem_init((sem_t*)_p, 0, val);    
}

Semaphore::~Semaphore()
{
    sem_destroy((sem_t*)_p);
}

void Semaphore::p()
{
    sem_wait((sem_t*)_p);
}

bool Semaphore::try_p()
{
    return sem_trywait((sem_t*)_p)? /*returned EAGAIN*/ false : /* returned zero */ true;
}

void Semaphore::v()
{
    sem_post((sem_t*)_p);
}


/// THREAD

void* _linux_Thread_run( void * arg )
{
    ((Thread*) arg)->entry();
    return 0;
}

Thread::Thread( Thread::Type t )
{
    pthread_attr_t attrs;
    pthread_attr_init(&attrs);

//     pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_JOINABLE);

    _p = new pthread_t;
    int status = pthread_create((pthread_t*)_p, &attrs, &_linux_Thread_run, this);

    if (status)
    {
        Logger::log("thread", "create failed");  
        perror("Thread::Thread");
        return;
    }

    if( t == LOWPRIO )
    {
        int policy;
        pthread_attr_getschedpolicy(&attrs, &policy);

        int prio = sched_get_priority_min(policy);
        pthread_setschedprio(*(pthread_t*)_p, prio);

    }else if ( t == FIFO )
    {
        // TODO SCHED_FIFO
    }
    
}

Thread::~Thread()
{
    // TODO define what to do 
    delete (pthread_t*) _p;
}


void Thread::join()
{
    pthread_join(*(pthread_t*)_p, 0 );
}

void Thread::entry()
{
    this->main();
}


/// MUTEX

Mutex::Mutex()
{
    _p = new pthread_mutex_t;
    pthread_mutex_init((pthread_mutex_t*) _p, 0);
}

Mutex::~Mutex()
{
    pthread_mutex_destroy((pthread_mutex_t*)_p);
    delete (pthread_mutex_t*)_p;
}

bool Mutex::tryLock()
{
    return pthread_mutex_trylock((pthread_mutex_t*)_p) == 0;
}

void Mutex::lock()
{
    pthread_mutex_lock((pthread_mutex_t*)_p);
}

void Mutex::unlock()
{
    pthread_mutex_unlock((pthread_mutex_t*)_p);
}


__HYDRUS_PLATFORM_END
