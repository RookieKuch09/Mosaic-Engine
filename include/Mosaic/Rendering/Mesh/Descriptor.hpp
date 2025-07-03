#pragma once

#include <Mosaic/Rendering/Mesh/Attributes.hpp>
#include <Mosaic/Rendering/Mesh/Flags.hpp>

namespace Mosaic
{
    template <std::size_t N>
    struct MeshDescriptorReflection
    {
        std::size_t VertexSize;
        std::size_t AttributeCount;

        MeshPrimitive Primitive;
        MeshIndexing Indexing;

        std::array<MeshAttributeReflection, N> AttributeReflections;
    };

    template <typename _AttributePack, typename _Flags>
    requires IsMeshAttributePackType<_AttributePack> && IsMeshFlagsType<_Flags>
    class MeshDescriptor
    {
    private:
        template <typename... _Attributes, std::size_t... Is>
        static consteval std::array<MeshAttributeReflection, sizeof...(_Attributes)> ReflectAttributes(std::tuple<_Attributes...>, std::index_sequence<Is...>)
        {
            return {_Attributes::Reflect()...};
        }

    public:
        MeshDescriptor() = delete;

        using AttributePack = _AttributePack;
        using Flags = _Flags;

        static constexpr std::size_t VertexSize = _AttributePack::VertexSize;

        static consteval MeshDescriptorReflection<AttributePack::AttributeCount> Reflect()
        {
            std::array<MeshAttributeReflection, AttributePack::AttributeCount> reflections{};

            std::size_t i = 0;

            auto function = [&]<typename T>() consteval
            {
                reflections[i++] = T::Reflect();
            };

            AttributePack::ForEach(function);

            return MeshDescriptorReflection<AttributePack::AttributeCount>{
                .VertexSize = VertexSize,
                .AttributeCount = AttributePack::AttributeCount,
                .Primitive = Flags::Primitive,
                .Indexing = Flags::Indexing,
                .AttributeReflections = reflections,
            };
        }
    };

    template <typename>
    inline constexpr bool IsMeshDescriptorType = false;

    template <typename _AttributePack, typename _Flags>
    inline constexpr bool IsMeshDescriptorType<MeshDescriptor<_AttributePack, _Flags>> = true;
}