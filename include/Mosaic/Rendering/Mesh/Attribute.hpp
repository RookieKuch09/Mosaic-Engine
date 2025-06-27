#pragma once

#include <Mosaic/Rendering/Mesh/Scalar.hpp>

#include <Mosaic/Utilities/Enumerator.hpp>

namespace Mosaic
{
    template <auto S, typename T, std::size_t N = 1>
    struct MeshAttribute
    {
        using Semantic = Enumerator<S>;
        using ValueType = T;
        using TypeInfo = MeshAttributeScalar<T>;
        static constexpr std::size_t Count = N;
    };
}