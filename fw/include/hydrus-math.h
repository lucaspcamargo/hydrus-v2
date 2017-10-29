#pragma once

#include "hydrus-config.h"

__HYDRUS_BEGIN

template <typename T>
T hMax( const T &a, const T &b )
{
    return (b>a)? b : a;
}

template <typename T>
T hMin( const T &a, const T &b )
{
    return (b<a)? b : a;
}

template <typename T>
T hClamp( const T &v )
{
    return hMin( (T)(1), hMax((T)(0), v) );
}

__HYDRUS_END
