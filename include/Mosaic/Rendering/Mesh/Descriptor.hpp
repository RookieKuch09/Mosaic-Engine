#pragma once

#include <Mosaic/Rendering/Mesh/Validation.hpp>

#include <array>

namespace Mosaic
{
    template <typename... A>
    requires MeshValidation::NoDuplicates<typename MeshValidation::ExtractSemantics<A...>::Type>::Value
    class MeshDescriptor
    {
    public:
    private:
    public:
        static constexpr std::size_t AttributeCount = sizeof...(A);
        static constexpr std::size_t Stride = ((A::TypeInfo::Size * A::TypeInfo::Count * A::Count) + ...);

        template <auto S>
        consteval static std::size_t GetOffset();

        template <auto S>
        consteval static std::size_t GetIndex();

        template <auto S>
        using Attribute = std::tuple_element_t<GetIndex<S>(), std::tuple<A...>>;

        using Attributes = std::tuple<A...>;

    private:
        template <auto S, std::size_t I = 0>
        consteval static std::size_t GetIndexHelper();
        consteval static std::array<std::size_t, AttributeCount> GetOffsets();

        static constexpr std::array<std::size_t, AttributeCount> Offsets = GetOffsets();
    };
}

#include <Mosaic/Inline/Rendering/Mesh/Descriptor.inl>