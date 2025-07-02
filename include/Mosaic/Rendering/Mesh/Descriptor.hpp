#pragma once

#include <Mosaic/Rendering/Mesh/Attributes.hpp>
#include <Mosaic/Rendering/Mesh/Flags.hpp>

namespace Mosaic
{
    struct MeshDescriptorReflection
    {
        std::size_t VertexSize;
        std::size_t AttributeCount;

        MeshPrimitive Primitive;
        MeshIndexing Indexing;
    };

    template <typename _AttributePack, typename _Flags>
    requires IsMeshAttributePackType<_AttributePack> && IsMeshFlagsType<_Flags>
    class MeshDescriptor
    {
    public:
        MeshDescriptor() = delete;

        using AttributePack = _AttributePack;
        using Flags = _Flags;

        static constexpr std::size_t VertexSize = _AttributePack::VertexSize;

        static consteval MeshDescriptorReflection Reflect()
        {
            return MeshDescriptorReflection{
                .VertexSize = VertexSize,
                .AttributeCount = AttributePack::AttributeCount,
                .Primitive = Flags::Primitive,
                .Indexing = Flags::Indexing,
            };
        }
    };

    template <typename>
    inline constexpr bool IsMeshDescriptorType = false;

    template <typename _AttributePack, typename _Flags>
    inline constexpr bool IsMeshDescriptorType<MeshDescriptor<_AttributePack, _Flags>> = true;
}