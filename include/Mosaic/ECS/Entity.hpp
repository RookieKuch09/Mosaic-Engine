#pragma once

#include <Mosaic/Macros/Exposure.hpp>

#include <cstdint>
#include <functional>

namespace Mosaic
{
    struct MOSAIC_PUBLIC_EXPOSURE EntityHandle
    {
        std::uint32_t ID;
        std::uint32_t Generation;

        bool operator==(const EntityHandle& other) const;
    };
}

namespace std
{
    template <>
    struct hash<Mosaic::EntityHandle>
    {
        std::size_t operator()(const Mosaic::EntityHandle& handle) const noexcept
        {
            std::size_t h1 = std::hash<std::uint32_t>{}(handle.ID);
            std::size_t h2 = std::hash<std::uint32_t>{}(handle.Generation);

            return h1 ^ (h2 << 1);
        }
    };
}