#pragma once

struct Waypoint
{
    double longitude, latitude;
    double customRadius;
    bool acquire;
};

#include <vector>

typedef std::vector<Waypoint> Waypoints;
