#pragma once

#include <mosaic/api/exposure.hpp>

#include <cstdint>

namespace Mosaic
{
    using EntityID = std::uint32_t;
    using EntityGeneration = std::uint32_t;

    struct MOSAIC_PUBLIC_EXPOSURE Entity
    {
        EntityID ID;
        EntityGeneration Generation;

        auto operator==(const Entity& other) const -> bool;
    };
}