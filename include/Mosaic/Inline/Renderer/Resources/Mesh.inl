#pragma once

#include <Mosaic/Renderer/Resources/Mesh.hpp>

namespace Mosaic
{
    template <typename... Attributes>
    MeshDescriptorInfo<MeshDescriptor<Attributes...>::AttributeCount> MeshDescriptor<Attributes...>::GetDescriptorInfo()
    {
        using Tuple = std::tuple<Attributes...>;

        static constexpr auto makeInfoTable = []<std::size_t... Is>(std::index_sequence<Is...>)
        {
            return std::array<MeshAttributeInfo, AttributeCount>{
                MeshAttributeInfo{
                    .Index = Is,
                    .Offset = Offsets[Is],
                    .Count = std::tuple_element_t<Is, Tuple>::Count,
                    .TypeSize = std::tuple_element_t<Is, Tuple>::TypeSize,
                    .TotalSize = std::tuple_element_t<Is, Tuple>::TotalSize,
                    .SemanticName = std::string(magic_enum::enum_name(std::tuple_element_t<Is, Tuple>::SemanticValue)),
                    .TypeName = boost::typeindex::type_id<typename std::tuple_element_t<Is, Tuple>::Type>().pretty_name()}...};
        };

        static const auto infoTable = makeInfoTable(std::make_index_sequence<AttributeCount>{});

        return MeshDescriptorInfo<AttributeCount>{
            .Attributes = infoTable,
            .Stride = Stride,
            .AttributeCount = AttributeCount,
        };
    }
}