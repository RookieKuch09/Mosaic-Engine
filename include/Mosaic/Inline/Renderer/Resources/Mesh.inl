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
            static_assert(I < AttributeCount, "MeshDescriptor::GetIndex: Semantic not found in descriptor");
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

    template <typename D>
    template <auto S, typename T, std::size_t N>
    void Mesh<D>::SetAttributeData(const FixedAttributeData<S, T, N>& attribute)
    {
        SetAttributeData<S, T, N>(attribute.Data);
    }

    template <typename D>
    template <auto S, typename T, std::size_t N>
    void Mesh<D>::SetAttributeData(const std::array<T, N>& data)
    {
        using Attribute = typename D::template Attribute<S>;

        constexpr std::size_t stride = D::Stride;
        constexpr std::size_t offset = D::template GetOffset<S>();
        constexpr std::size_t typeSize = MeshAttributeTypeInfo<T>::Size;
        constexpr std::size_t attributeCount = Attribute::Count;

        std::byte* base = mData.data();
        const T* src = data.data();

        for (std::size_t i = 0; i < mVertexCount; i++)
        {
            std::byte* dst = base + i * stride + offset;
            std::memcpy(dst, &src[i * attributeCount], typeSize * attributeCount);
        }
    }

    template <typename D>
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
    void Mesh<D>::SetMeshData(Attributes&&... attributes)
    {
        using FirstAttr = std::decay_t<std::tuple_element_t<0, std::tuple<Attributes...>>>;
        constexpr std::size_t vertexCount = GetAttributeVertexCount<D, FirstAttr>::Count;

        static_assert(vertexCount > 0, "Cannot provide empty data to mesh");

        mVertexCount = vertexCount;
        mData.resize(mVertexCount * D::Stride);

        (SetAttributeData(std::forward<Attributes>(attributes)), ...);
    }
}