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