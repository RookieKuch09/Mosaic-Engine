#pragma once

#include <Mosaic/Macros/Exposure.hpp>

#include <cstdint>

namespace Mosaic
{
    using EntityID = std::uint32_t;
    using EntityGeneration = std::uint32_t;

    struct MOSAIC_PUBLIC_EXPOSURE Entity
    {
        EntityID ID;
        EntityGeneration Generation;

        bool operator==(const Entity& other) const;
    };
}