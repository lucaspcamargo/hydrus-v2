/**
 * @file blackboard.h
 * @author Lucas Camargo <camargo@lisha.ufsc.br>
 * @brief The blackboard keeps all important system state
 *
 * This blackboard implementation is double-buffered.
 * While the front-buffer is locked for writing, the back-buffer is free for  
 * read-only access. The locking mechanism is a variant of the Readers-Writer
 * lock, with special considerations for swapping the front and the back buffers.
 */

#pragma once

#include "hydrus-config.h"

__HYDRUS_BEGIN

enum SystemState {
    SS_INITIALIZING,
    SS_READY,
    SS_NAVIGATING,
    SS_SHUTDOWN,
    SS_HALTED,
    SS_FAULT
};
const char * toString(SystemState st);

enum NavigationState {
    NS_NOT_NAVIGATING,  // navigation not ongoing
    NS_ALIGN,           // point vessel towards waypoint
    NS_ALIGN_WAIT,      // vessel is pointing towards waypoint
    NS_TRAVERSE,        // vessel moves towards waypoint
    NS_ARRIVAL_WAIT,    // vessel in in radius of waypoint
    NS_ACQUIRE,         // vessel is acquiring data
    NS_HOMING_EMERGENCY // vessel is returning home because something is not right
};
const char * toString(NavigationState st);

typedef SystemState State;


struct Blackboard
{
    struct sys_struct {

        SystemState state = SS_INITIALIZING;

        bool reboot;

        float battVoltage;
        float battLevel;
        float coreTemp;
        float cpuLoad;
    } sys;

    struct nav_struct {

        NavigationState state = NS_NOT_NAVIGATING;

        //State gpsState = SS_INITIALIZING;
        bool gpsHasFix = false;
        float gpsLat = 0;
        float gpsLon = 0;

        float geoHeading = 0;
        float distanceFromNextWaypoint;

        bool rcMode;
        float rcVecX;
        float rcVecY;

        struct motor_struct {
            bool enabled = false;
            float spdL = 0;
            float spdR = 0;
        } motor;

        bool simMode = false;

    } nav;

    struct sensors_struct
    {
        float waterTurb;
        float waterTemp;
        float waterPH;

        float imuHeading;

        float weatherTemp;
        float weatherHum;
        float weatherLight;

        float sonarA;
        float sonarB;
        float sonarC;
        float sonarD;

    } sensors;

    struct comm_struct
    {
        bool connected;
    } comm;



    //operational

    class Transactional {
    public:
        
        /**
        * @brief initiate a blackboard transaction
        */
        static void begin( bool read = false ); // read-write by default
        
        /**
        * @brief ends a blackboard transaction
        */
        static void end( bool read = false );

    } trans;

    /**
     * @brief initializes blackboard memory, and double buffering
     */
    static void init();

};

/** @brief read-write access point to the blackboard (valid only on locking) */
extern Blackboard* BB;

/** @brief read-only access point to blackboard (backbuffer) */
extern const Blackboard* BBro;


class BlackboardTransaction
{
public:
    BlackboardTransaction( bool ro = false ) : m_ro(ro)
    {
        BB->trans.begin(m_ro);
    }
    
    ~BlackboardTransaction()
    {
        BB->trans.end(m_ro);
    }
    
    bool m_ro;    
};

__HYDRUS_END;
