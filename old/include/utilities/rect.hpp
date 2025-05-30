#pragma once

#include "utilities/numerics.hpp"
#include "utilities/vector.hpp"

namespace Mosaic::Systems::Internal::Types
{
    struct Rectangle
    {
        Vec2<UI32> Position;
        Vec2<UI32> Size;
    };
}