#pragma once

#include <Mosaic/Rendering/Mesh/Descriptor.hpp>

#include <utility>

namespace Mosaic
{
    template <typename... A> requires MeshValidation::NoDuplicates<typename MeshValidation::ExtractSemantics<A...>::Type>::Value
    template <auto S, std::size_t I>
    consteval std::size_t MeshDescriptor<A...>::GetIndexHelper()
    {
        if constexpr (I >= AttributeCount)
        {
            static_assert(I < AttributeCount, "MeshDescriptor::GetIndex: Semantic ! found in descriptor");
        }
        else if constexpr (std::tuple_element_t<I, std::tuple<A...>>::Semantic::Value == S)
        {
            return I;
        }
        else
        {
            return GetIndexHelper<S, I + 1>();
        }
    }

    template <typename... A> requires MeshValidation::NoDuplicates<typename MeshValidation::ExtractSemantics<A...>::Type>::Value
    template <auto S>
    consteval std::size_t MeshDescriptor<A...>::GetOffset()
    {
        return Offsets[GetIndex<S>()];
    }

    template <typename... A> requires MeshValidation::NoDuplicates<typename MeshValidation::ExtractSemantics<A...>::Type>::Value
    template <auto S>
    consteval std::size_t MeshDescriptor<A...>::GetIndex()
    {
        return GetIndexHelper<S>();
    }

    template <typename... A> requires MeshValidation::NoDuplicates<typename MeshValidation::ExtractSemantics<A...>::Type>::Value
    consteval std::array<std::size_t, MeshDescriptor<A...>::AttributeCount> MeshDescriptor<A...>::GetOffsets()
    {
        std::array<std::size_t, AttributeCount> array = {};

        std::size_t offset = 0;
        std::size_t i = 0;

        ((array[i++] = std::exchange(offset, offset + (A::TypeInfo::Size * A::TypeInfo::Count * A::Count))), ...);

        return array;
    }
}