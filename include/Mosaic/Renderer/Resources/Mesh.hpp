#pragma once

#include <Mosaic/Debug/Console.hpp>

#include <Mosaic/Utilities/Enumerator.hpp>

#include <array>
#include <cstring>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

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
        using Semantic = Enumerator<S>;

        using ValueType = T;
        using TypeInfo = MeshAttributeTypeInfo<T>;

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

    template <auto S, typename T, std::size_t N>
    struct FixedAttributeData
    {
        using Semantic = Enumerator<S>;
        using Type = T;

        static constexpr std::size_t Size = N;
        static constexpr std::size_t Count = N * MeshAttributeTypeInfo<T>::Count;

        std::array<T, N> Data;
    };

    template <typename... Ts>
    struct ExtractAttributeSemantics
    {
        using List = std::tuple<typename Ts::Semantic...>;
    };

    template <typename Descriptor>
    struct ExtractDescriptorSemantics;

    template <typename... A>
    struct ExtractDescriptorSemantics<Mosaic::MeshDescriptor<A...>>
    {
        using List = std::tuple<typename A::Semantic...>;
    };

    template <typename Expected, typename Actual>
    struct TuplesMatch
    {
        static constexpr bool Value = false;
    };

    template <typename... Expected, typename... Actual>
    struct TuplesMatch<std::tuple<Expected...>, std::tuple<Actual...>>
    {
        static constexpr bool Value = (std::is_same_v<Expected, Actual> && ...);
    };

    template <typename Descriptor, typename Attr>
    struct GetAttributeVertexCount;

    template <typename D, auto S, typename T, std::size_t N>
    struct GetAttributeVertexCount<D, FixedAttributeData<S, T, N>>
    {
    private:
        static constexpr std::size_t descriptorCount = D::template Attribute<S>::Count;
        static constexpr bool valid = (N % descriptorCount == 0);

    public:
        static_assert(valid, "Attribute data size must be divisible by the number of components per vertex.");
        static constexpr std::size_t Count = N / descriptorCount;
    };

    template <typename Descriptor, std::size_t Expected, typename... Attrs>
    struct AllVertexCountsMatch;

    template <typename Descriptor, std::size_t Expected>
    struct AllVertexCountsMatch<Descriptor, Expected>
    {
        static constexpr bool Value = true;
    };

    template <typename Descriptor, std::size_t Expected, typename First, typename... Rest>
    struct AllVertexCountsMatch<Descriptor, Expected, First, Rest...>
    {
        static constexpr std::size_t Count = GetAttributeVertexCount<Descriptor, First>::Count;
        static constexpr bool Value = (Count == Expected) &&
                                      AllVertexCountsMatch<Descriptor, Expected, Rest...>::Value;
    };

    template <typename D>
    class Mesh
    {
    public:
        using Descriptor = D;

        Mesh() : mVertexCount(0)
        {
        }

        template <typename... Attributes>
        requires(
            TuplesMatch<typename ExtractDescriptorSemantics<D>::List, typename ExtractAttributeSemantics<std::decay_t<Attributes>...>::List>::Value &&
            AllVertexCountsMatch<D,
                                 GetAttributeVertexCount<D, std::decay_t<std::tuple_element_t<0, std::tuple<Attributes...>>>>::Count,
                                 std::decay_t<Attributes>...>::Value &&
            (std::same_as<
                 typename std::decay_t<Attributes>::Type,
                 typename D::template Attribute<std::decay_t<Attributes>::Semantic::Value>::ValueType> &&
             ...))
        void SetMeshData(Attributes&&... attributes);

    private:
        template <auto S, typename T, std::size_t N>
        void SetAttributeData(const FixedAttributeData<S, T, N>& attribute);

        template <auto S, typename T, std::size_t N>
        void SetAttributeData(const std::array<T, N>& data);

        std::vector<std::byte> mData;
        std::size_t mVertexCount;
    };
}

#include <Mosaic/Inline/Renderer/Resources/Mesh.inl>