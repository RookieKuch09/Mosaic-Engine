#pragma once

#include <Mosaic/Rendering/Mesh/Validation.hpp>

#include <cstdint>

namespace Mosaic
{
    template <typename _Descriptor>
    requires IsMeshDescriptorType<_Descriptor>::value
    class Mesh
    {
    public:
        using Descriptor = _Descriptor;

        // TODO: implement mesh
    };

    class MeshHandle
    {
    public:
        MeshHandle(const MeshHandle&) = default;
        MeshHandle(MeshHandle&&) noexcept = default;

        MeshHandle& operator=(const MeshHandle&) = default;
        MeshHandle& operator=(MeshHandle&&) noexcept = default;

        bool operator==(const MeshHandle&) const = default;
        bool operator!=(const MeshHandle&) const = default;

    private:
        MeshHandle(const std::uint32_t id);

        std::uint32_t mID;
    };
}