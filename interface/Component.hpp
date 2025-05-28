#pragma once

#if __cplusplus >= 202300L

#include <Mosaic/include/frontend/instance.hpp>
#include <Mosaic/include/frontend/logging.hpp>

namespace Mosaic
{
    using namespace Frontend;
}

#else

#error Mosaic requires a compiler compatible with the C++23 standard

#endif