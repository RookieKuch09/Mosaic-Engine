#pragma once

#if __cplusplus >= 202300L

#include <Mosaic/include/frontend/instance.hpp>
#include <Mosaic/include/frontend/main.hpp>
#include <Mosaic/include/utilities/logging.hpp>

namespace Mosaic
{
    using namespace Frontend;
    using namespace Utilities;
}

#else

#error Mosaic requires a compiler compatible with the C++23 standard

#endif