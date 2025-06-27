#pragma once

#include <Mosaic/Rendering/Mesh/Mesh.hpp>

#include <cstring>

namespace Mosaic
{
    template <typename D>
    Mesh<D>::Mesh()
        : mVertexCount(0)
    {
    }

    template <typename D>
    template <typename... Attributes> requires MeshValidation::ValidMeshData<D, Attributes...>
    void Mesh<D>::SetMeshData(Attributes&&... attributes)
    {
        using FirstAttr = std::decay_t<std::tuple_element_t<0, std::tuple<Attributes...>>>;
        constexpr std::size_t vertexCount = MeshValidation::AttributeVertexInfo<D, FirstAttr>::VertexCount;

        static_assert(vertexCount > 0, "Cannot provide empty data to mesh");

        mVertexCount = vertexCount;
        mData.resize(mVertexCount * D::Stride);

        (SetAttributeData(std::forward<Attributes>(attributes)), ...);
    }

    template <typename D>
    template <auto S, typename T, std::size_t N>
    void Mesh<D>::SetAttributeData(const MeshSemanticData<S, T, N>& attribute)
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
        constexpr std::size_t attributeCount = Attribute::Count;

        std::byte* base = mData.data();
        const T* src = data.data();

        for (std::size_t i = 0; i < mVertexCount; i++)
        {
            std::byte* dst = base + i * stride + offset;
            std::memcpy(dst, &src[i * attributeCount], sizeof(T) * attributeCount);
        }
    }
}