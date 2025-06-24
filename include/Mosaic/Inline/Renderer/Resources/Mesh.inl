#pragma once

#include <Mosaic/Renderer/Resources/Mesh.hpp>

namespace Mosaic
{
    template <typename... A>
    template <auto S, std::size_t I>
    consteval std::size_t MeshDescriptor<A...>::GetIndexHelper()
    {
        if constexpr (I >= AttributeCount)
        {
            static_assert(I < AttributeCount, "Semantic not found in MeshDescriptor");
        }
        else if constexpr (std::tuple_element_t<I, std::tuple<A...>>::Semantic == S)
        {
            return I;
        }
        else
        {
            return GetIndexHelper<S, I + 1>();
        }
    }

    template <typename... A>
    MeshDescriptorInfo<MeshDescriptor<A...>::AttributeCount> MeshDescriptor<A...>::GetDescriptorInfo()
    {
        using Tuple = std::tuple<A...>;

        static constexpr auto makeInfoTable = []<std::size_t... I>(std::index_sequence<I...>)
        {
            return std::array<MeshAttributeInfo, AttributeCount>{
                MeshAttributeInfo{
                    .Index = I,
                    .Offset = Offsets[I],
                    .Count = std::tuple_element_t<I, Tuple>::ElementCount,
                    .TypeSize = sizeof(typename std::tuple_element_t<I, Tuple>::Type),
                    .TotalSize = sizeof(typename std::tuple_element_t<I, Tuple>::Type) * std::tuple_element_t<I, Tuple>::ElementCount,
                    .SemanticName = std::string(magic_enum::enum_name(std::tuple_element_t<I, Tuple>::Semantic)),
                    .TypeName = boost::typeindex::type_id<typename std::tuple_element_t<I, Tuple>::Type>().pretty_name()}...};
        };

        static const auto infoTable = makeInfoTable(std::make_index_sequence<AttributeCount>{});

        return MeshDescriptorInfo<AttributeCount>{
            .Attributes = infoTable,
            .Stride = Stride,
            .AttributeCount = AttributeCount,
        };
    }

    template <typename... A>
    template <auto S>
    consteval std::size_t MeshDescriptor<A...>::GetOffset()
    {
        return Offsets[GetIndex<S>()];
    }

    template <typename... A>
    template <auto S>
    consteval std::size_t MeshDescriptor<A...>::GetIndex()
    {
        return GetIndexHelper<S>();
    }
}