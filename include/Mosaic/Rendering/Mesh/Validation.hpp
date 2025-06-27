#pragma once

#include <tuple>

namespace Mosaic
{
    namespace MeshValidation
    {
        template <typename T>
        struct ExtractSemanticType
        {
            using Type = typename std::decay_t<T>::Semantic;
        };

        template <typename... Ts>
        struct ExtractSemantics
        {
            using Type = std::tuple<typename ExtractSemanticType<Ts>::Type...>;
        };

        template <typename... Ts>
        struct ExtractSemantics<std::tuple<Ts...>>
        {
            using Type = std::tuple<typename ExtractSemanticType<std::decay_t<Ts>>::Type...>;
        };

        template <typename T, typename Tuple>
        struct Contains;

        template <typename T>
        struct Contains<T, std::tuple<>>
        {
            static constexpr bool Value = false;
        };

        template <typename T, typename U, typename... Rest>
        struct Contains<T, std::tuple<U, Rest...>>
        {
            static constexpr bool Value = std::is_same_v<T, U> || Contains<T, std::tuple<Rest...>>::Value;
        };

        template <typename Tuple>
        struct NoDuplicates;

        template <>
        struct NoDuplicates<std::tuple<>>
        {
            static constexpr bool Value = true;
        };

        template <typename T, typename... Rest>
        struct NoDuplicates<std::tuple<T, Rest...>>
        {
            static constexpr bool Value = !Contains<T, std::tuple<Rest...>>::Value && NoDuplicates<std::tuple<Rest...>>::Value;
        };
    }

    template <typename... A>
    requires MeshValidation::NoDuplicates<typename MeshValidation::ExtractSemantics<A...>::Type>::Value
    class MeshDescriptor;

    namespace MeshValidation
    {
        template <typename... As>
        struct ExtractSemantics<Mosaic::MeshDescriptor<As...>>
        {
            using Type = std::tuple<typename As::Semantic...>;
        };

        template <typename T1, typename T2>
        struct TuplesMatch
        {
            static constexpr bool Value = false;
        };

        template <typename... T1s, typename... T2s>
        struct TuplesMatch<std::tuple<T1s...>, std::tuple<T2s...>>
        {
            static constexpr bool Value = (std::is_same_v<T1s, T2s> && ...);
        };

        template <typename D, typename RA>
        struct AttributeVertexInfo
        {
            using A = std::decay_t<RA>;
            using Attribute = typename D::template Attribute<A::Semantic::Value>;

            static_assert(A::Size % Attribute::Count == 0, "Attribute data size must be divisible by per-vertex component count");

            static constexpr std::size_t VertexCount = A::Size / Attribute::Count;
            static constexpr bool Valid = true;
        };

        template <typename Descriptor, std::size_t Expected, typename... Attrs>
        struct VertexCountsMatch
        {
            static constexpr bool Value = true;
        };

        template <typename D, std::size_t Expected, typename FirstRaw, typename... Rest>
        struct VertexCountsMatch<D, Expected, FirstRaw, Rest...>
        {
            using First = std::decay_t<FirstRaw>;

            static constexpr std::size_t Actual = AttributeVertexInfo<D, First>::VertexCount;
            static constexpr bool Value = (Actual == Expected) && VertexCountsMatch<D, Expected, Rest...>::Value;
        };

        template <typename D, typename... Attrs>
        static constexpr bool ValidMeshData = TuplesMatch<typename ExtractSemantics<D>::Type, typename ExtractSemantics<std::tuple<Attrs...>>::Type>::Value &&
                                              VertexCountsMatch<D, AttributeVertexInfo<D, std::decay_t<std::tuple_element_t<0, std::tuple<Attrs...>>>>::VertexCount, Attrs...>::Value &&
                                              (std::same_as<typename std::decay_t<Attrs>::Type, typename D::template Attribute<std::decay_t<Attrs>::Semantic::Value>::ValueType> && ...);
    }
}