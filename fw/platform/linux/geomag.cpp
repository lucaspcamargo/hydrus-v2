#include "platform/geomag.h"

// extern "C" {
// #include "./geomag/wmm-wrapper.h"
// }

// wmm_t wmm;

__HYDRUS_PLATFORM_BEGIN

void GeoMag::init()
{
//     wmm_init(&wmm);
}


float GeoMag::declination(float lon_deg, float lat_deg, float alt_km )
{
//     wmm_declination_deg(wmm, lon_deg, lat_deg, alt_km);
    return 0;
}

__HYDRUS_PLATFORM_END

// extern "C" {
// #include "./geomag/wmm-wrapper.c"
// #include "./geomag/wmm.c"
// }
