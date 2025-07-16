#pragma once

#include <cstdint>

namespace Mosaic::Systems::ECS
{
    struct EntityHandle
    {
        std::uint32_t ID;
        std::uint32_t Generation;
    };
}