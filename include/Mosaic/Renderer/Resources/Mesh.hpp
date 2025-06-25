#pragma once

#include <array>
#include <tuple>
#include <type_traits>
#include <utility>

#include <glm/glm.hpp>

namespace Mosaic
{
    template <typename T>
    struct MeshAttributeTypeInfo;

    template <typename T> requires std::is_arithmetic_v<T>
    struct MeshAttributeTypeInfo<T>
    {
        using Type = T;

        static constexpr std::size_t Size = sizeof(T);
        static constexpr std::size_t Count = 1;
    };

    template <typename T> requires std::is_arithmetic_v<T>
    struct MeshAttributeTypeInfo<glm::vec<2, T>>
    {
        using Type = T;

        static constexpr std::size_t Size = sizeof(T);
        static constexpr std::size_t Count = 2;
    };

    template <typename T> requires std::is_arithmetic_v<T>
    struct MeshAttributeTypeInfo<glm::vec<3, T>>
    {
        using Type = T;

        static constexpr std::size_t Size = sizeof(T);
        static constexpr std::size_t Count = 3;
    };

    template <typename T> requires std::is_arithmetic_v<T>
    struct MeshAttributeTypeInfo<glm::vec<4, T>>
    {
        using Type = T;

        static constexpr std::size_t Size = sizeof(T);
        static constexpr std::size_t Count = 4;
    };

    template <auto S, typename T, std::size_t N = 1>
    struct MeshAttribute
    {
        using SemanticType = decltype(S);
        using TypeInfo = MeshAttributeTypeInfo<T>;

        static constexpr auto Semantic = S;
        static constexpr std::size_t Count = N;
    };

    template <typename... A>
    class MeshDescriptor
    {
    public:
        static constexpr std::size_t AttributeCount = sizeof...(A);
        static constexpr std::size_t Stride = ((A::TypeInfo::Size * A::TypeInfo::Count * A::Count) + ...);

        template <auto S>
        consteval static std::size_t GetOffset();

        template <auto S>
        consteval static std::size_t GetIndex();

        template <auto S>
        using Attribute = std::tuple_element_t<MeshDescriptor::template GetIndex<S>(), std::tuple<A...>>;

    private:
        template <auto S, std::size_t I = 0>
        consteval static std::size_t GetIndexHelper();

        static constexpr std::array<std::size_t, AttributeCount> Offsets = []
        {
            std::array<std::size_t, AttributeCount> arr{};

            std::size_t offset = 0;
            std::size_t i = 0;

            ((arr[i++] = std::exchange(offset, offset + (A::TypeInfo::Size * A::TypeInfo::Count * A::Count))), ...);

            return arr;
        }();
    };
}

#include <Mosaic/Inline/Renderer/Resources/Mesh.inl>