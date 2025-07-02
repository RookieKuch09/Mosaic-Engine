#pragma once

#include <cstddef>

namespace Mosaic
{
    enum class MeshPrimitive
    {
        Points,
        Lines,
        Triangles,
    };

    template <MeshPrimitive _Primitive>
    struct MeshPrimitiveRequirements;

    template <>
    struct MeshPrimitiveRequirements<MeshPrimitive::Points>
    {
        static constexpr std::size_t MinimumVertices = 1;
    };

    template <>
    struct MeshPrimitiveRequirements<MeshPrimitive::Lines>
    {
        static constexpr std::size_t MinimumVertices = 2;
    };

    template <>
    struct MeshPrimitiveRequirements<MeshPrimitive::Triangles>
    {
        static constexpr std::size_t MinimumVertices = 3;
    };

    enum class MeshIndexing
    {
        Disabled,
        U8,
        U16,
        U32,
    };

    template <MeshPrimitive _Primitive, MeshIndexing _Indexing>
    class MeshFlags
    {
    public:
        MeshFlags() = delete;

        static constexpr MeshPrimitive Primitive = _Primitive;
        static constexpr MeshIndexing Indexing = _Indexing;
    };

    template <typename>
    inline constexpr bool IsMeshFlagsType = false;

    template <MeshPrimitive _Primitive, MeshIndexing _Indexing>
    inline constexpr bool IsMeshFlagsType<MeshFlags<_Primitive, _Indexing>> = true;
}