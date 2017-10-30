#pragma once

#include <cstdlib>
#include <cstdio>
#include <fstream>

float readSOCTemperature()
{
    char temp_raw[6];
    
    FILE* fp = fopen("/sys/class/thermal/thermal_zone0/temp", "r"); 
    fgets(temp_raw, 5, fp);
    fclose(fp);
    
    int temp = atoi(temp_raw);
    return temp * 0.01f; 
}

float readCPULoad()
{
    float ret = 0;
    
    static bool initialized = false;
    static int ousr, onice, osys, oidle;
    
    int usr, nice, sys, idle;
    
    std::fstream in("/proc/stat", std::ios_base::in);
    in.ignore(4);
    in >> usr >> nice >> sys >> idle;
    in.close();
    
    if(initialized)
    {
        int dusr = usr - ousr;
        int dnice = nice - onice;
        int dsys = sys - osys;
        int didle = idle - oidle;
        
        ret = (dusr+dnice+dsys)*100.0f/(dusr+dnice+dsys+didle);
        
    }
    else initialized = true;
    
    ousr = usr;
    onice = nice;
    osys = sys;
    oidle = idle;   
    
    return ret;
}
