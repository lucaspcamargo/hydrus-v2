#include "blackboard.h"

#include "platform/mutex.h"

__HYDRUS_BEGIN

Blackboard bb[2];
volatile int _bbRWindex;
volatile int _bbReaderCount;

P::Mutex *_bbWriteLock;
P::Mutex *_bbWantsToSwap;
P::Mutex *_bbReaderControlLock;
P::Mutex *_bbPointerControlLock;

void Blackboard::init()
{        
    _bbRWindex = 0;
    _bbReaderCount = 0;
    
    bzero(bb, sizeof(Blackboard));
    bzero(bb+1, sizeof(Blackboard));
    
    _bbWriteLock = new P::Mutex();
    _bbWantsToSwap = new P::Mutex();
    _bbReaderControlLock = new P::Mutex();
    _bbPointerControlLock = new P::Mutex();
}

void Blackboard::Transactional::begin( bool ro )
{
    if(ro)
    {
        // if writer has locked this mutex, readers are blocked from beginning 
        // new transactions before all the current ones finish and release the
        // PointerControlLock
        _bbWantsToSwap->lock();
        _bbWantsToSwap->unlock();
        
        P::MutexLock l( _bbReaderControlLock );        
        _bbReaderCount ++;
        
        if(_bbReaderCount == 1)
            _bbPointerControlLock->lock(); // needs to hold pointers in place
    }
    else
    {
        _bbWriteLock->lock();
    }
}

void Blackboard::Transactional::end( bool ro )
{
    if(ro)
    {
        P::MutexLock l( _bbReaderControlLock );
        _bbReaderCount --;
        
        if(_bbReaderCount == 0)
            _bbPointerControlLock->unlock(); // no need to control pointers anymore
    }
    else
    {
        bool needsToSwap = true; // if there was any modification
        
        if(needsToSwap)
        {
            _bbWantsToSwap->lock();
            _bbPointerControlLock->lock(); // need control of pointers
            
            memcpy( bb + (1-_bbRWindex), bb + _bbRWindex, sizeof(Blackboard) );
            _bbRWindex = 1-_bbRWindex; // swap index
            
            BB = bb + _bbRWindex; // set pointers
            BBro = bb + (1-_bbRWindex); // set all pointers
            
            _bbPointerControlLock->unlock(); // can forego control of pointers
            _bbWantsToSwap->unlock();            
        }
        
        _bbWriteLock->unlock(); // and let somebody else write for a change
    }
}


const char * toString(SystemState st)
{
    const char * const tbl[] =
    {   "INIT",
        "READY",
        "NAVIGATING",
        "SHUTDOWN",
        "HALTED",
        "FAULT"
    };

    return tbl[(int) st];
}


const char * toString(NavigationState st)
{
    const char * const tbl[] =
    {   "NOT NAVIGATING",
        "ALIGN",
        "ALIGN_WAIT",
        "TRAVERSE",
        "ARRIVAL_WAIT",
        "ACQUIRE",
        "HOMING EMERGENCY"
    };

    return tbl[(int) st];
}

__HYDRUS_END
