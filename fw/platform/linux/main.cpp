
#include "hydrus-config.h"
#include "platform/logger.h"

__HYDRUS_PLATFORM_BEGIN

    void init( int argc, char **argv );

__HYDRUS_PLATFORM_END


__HYDRUS_BEGIN
    int hydrus_main( );
__HYDRUS_END


int main( int argc, char ** argv )
{
    Hydrus::P::Logger::log("platform", "initializing", Hydrus::P::Logger::DEBUG);
    Hydrus::P::init(argc, argv);
    Hydrus::P::Logger::log("platform", "application started", Hydrus::P::Logger::DEBUG);
    int ret = Hydrus::hydrus_main();    
    Hydrus::P::Logger::log("platform", "application ended", Hydrus::P::Logger::WARNING);
    return ret;
}
