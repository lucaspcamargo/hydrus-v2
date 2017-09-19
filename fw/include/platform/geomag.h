#pragma once

#include "hydrus-config.h"

__HYDRUS_PLATFORM_BEGIN

class GeoMag {
    
    static void init();
    
    static float declination(float lon_deg, float lat_deg, float alt_km = 0.0f);
};

__HYDRUS_PLATFORM_END
