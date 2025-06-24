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
        const std::size_t Index;
        const std::size_t Offset;
        const std::size_t Count;
        const std::size_t TypeSize;
        const std::size_t TotalSize;
        const std::string SemanticName;
        const std::string TypeName;
    };

    template <std::size_t N>
    struct MeshDescriptorInfo
    {
        const std::array<MeshAttributeInfo, N> Attributes;

        const std::size_t Stride;
        const std::size_t AttributeCount;
    };

    template <auto S, typename T, std::size_t N>
    struct MeshAttribute
    {
        static constexpr auto Semantic = S;
        static constexpr std::size_t ElementCount = N;

        using Type = T;
    };

    template <typename... A>
    class MeshDescriptor
    {
    public:
        static constexpr std::size_t AttributeCount = sizeof...(A);
        static constexpr std::size_t Stride = ((sizeof(typename A::Type) * A::ElementCount) + ...);

        static MeshDescriptorInfo<AttributeCount> GetDescriptorInfo();

        template <auto S>
        consteval static std::size_t GetOffset();

        template <auto S>
        consteval static std::size_t GetIndex();

        template <auto S>
        using GetAttribute = std::tuple_element_t<MeshDescriptor::template GetIndex<S>(), std::tuple<A...>>;

    private:
        template <auto S, std::size_t I = 0>
        consteval static std::size_t GetIndexHelper();

        static constexpr std::array<std::size_t, AttributeCount> Offsets = []
        {
            std::array<std::size_t, AttributeCount> arr{};
            std::size_t offset = 0;
            std::size_t i = 0;
            ((arr[i++] = std::exchange(offset, offset + sizeof(typename A::Type) * A::ElementCount)), ...);
            return arr;
        }();
    };
}

#include <Mosaic/Inline/Renderer/Resources/Mesh.inl>