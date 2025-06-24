#pragma once

#include <array>
#include <string>
#include <tuple>
#include <utility>

#include <boost/type_index.hpp>

#include <magic_enum/magic_enum.hpp>

namespace Mosaic
{
    struct MeshAttributeInfo
    {
        std::size_t Index;
        std::size_t Offset;
        std::size_t Count;
        std::size_t TypeSize;
        std::size_t TotalSize;
        std::string SemanticName;
        std::string TypeName;
    };

    template <std::size_t N>
    struct MeshDescriptorInfo
    {
        std::array<MeshAttributeInfo, N> Attributes;

        std::size_t Stride;
        std::size_t AttributeCount;
    };

    template <auto Semantic, typename T, std::size_t N>
    struct MeshAttribute
    {
        static constexpr auto SemanticValue = Semantic;
        static constexpr std::size_t Count = N;
        static constexpr std::size_t TypeSize = sizeof(T);
        static constexpr std::size_t TotalSize = Count * TypeSize;

        using Type = T;
    };

    template <typename... Attributes>
    class MeshDescriptor
    {
    public:
        static constexpr std::size_t AttributeCount = sizeof...(Attributes);
        static constexpr std::size_t Stride = (Attributes::TotalSize + ...);

        template <auto Semantic>
        using GetAttribute = std::tuple_element_t<FindIndex<Semantic>(), std::tuple<Attributes...>>;

        static MeshDescriptorInfo<AttributeCount> GetDescriptorInfo();

    private:
        static constexpr std::array<std::size_t, AttributeCount> Offsets = []
        {
            std::array<std::size_t, AttributeCount> arr{};
            std::size_t offset = 0;
            std::size_t i = 0;
            ((arr[i++] = std::exchange(offset, offset + Attributes::TotalSize)), ...);
            return arr;
        }();
    };
}

#include <Mosaic/Inline/Renderer/Resources/Mesh.inl>