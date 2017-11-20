#pragma once

struct Waypoint
{
    double longitude, latitude;
    double custom_radius;
    bool acquire;
};

#include <vector>

typedef std::vector<Waypoint> Waypoints;
