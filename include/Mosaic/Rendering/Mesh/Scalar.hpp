#pragma once

#include <type_traits>

#include <glm/glm.hpp>

namespace Mosaic
{
    template <typename T>
    struct MeshAttributeScalar;

    template <typename T> requires std::is_arithmetic_v<T>
    struct MeshAttributeScalar<T>
    {
        using Type = T;
        static constexpr std::size_t Size = sizeof(T);
        static constexpr std::size_t Count = 1;
    };

    template <typename T> requires std::is_arithmetic_v<T>
    struct MeshAttributeScalar<glm::vec<2, T>>
    {
        using Type = T;
        static constexpr std::size_t Size = sizeof(T);
        static constexpr std::size_t Count = 2;
    };

    template <typename T> requires std::is_arithmetic_v<T>
    struct MeshAttributeScalar<glm::vec<3, T>>
    {
        using Type = T;
        static constexpr std::size_t Size = sizeof(T);
        static constexpr std::size_t Count = 3;
    };

    template <typename T> requires std::is_arithmetic_v<T>
    struct MeshAttributeScalar<glm::vec<4, T>>
    {
        using Type = T;
        static constexpr std::size_t Size = sizeof(T);
        static constexpr std::size_t Count = 4;
    };
}