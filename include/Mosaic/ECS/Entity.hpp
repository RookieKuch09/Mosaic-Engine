#pragma once

#include <cstdint>

namespace Mosaic::ECS
{
    struct Entity
    {
        std::uint32_t ID;
        std::uint32_t Generation;
    };
}