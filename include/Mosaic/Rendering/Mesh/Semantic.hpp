#pragma once

#include <Mosaic/Rendering/Mesh/Scalar.hpp>

#include <Mosaic/Utilities/Enumerator.hpp>

#include <array>

namespace Mosaic
{
    template <auto S, typename T, std::size_t N>
    struct MeshSemanticData
    {
        using Semantic = Enumerator<S>;
        using Type = T;

        static constexpr std::size_t Size = N;
        static constexpr std::size_t Count = N * MeshAttributeScalar<T>::Count;

        std::array<T, N> Data;
    };
}