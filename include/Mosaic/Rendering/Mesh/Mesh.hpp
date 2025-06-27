#pragma once

#include <Mosaic/Rendering/Mesh/Semantic.hpp>
#include <Mosaic/Rendering/Mesh/Validation.hpp>

#include <vector>

namespace Mosaic
{
    template <typename D>
    class Mesh
    {
    public:
        using Descriptor = D;

        Mesh();

        template <typename... Attributes> requires MeshValidation::ValidMeshData<D, Attributes...>
        void SetMeshData(Attributes&&... attributes);

    private:
        template <auto S, typename T, std::size_t N>
        void SetAttributeData(const MeshSemanticData<S, T, N>& attribute);

        template <auto S, typename T, std::size_t N>
        void SetAttributeData(const std::array<T, N>& data);

        std::vector<std::byte> mData;
        std::size_t mVertexCount;
    };
}

#include <Mosaic/Inline/Rendering/Mesh/Mesh.inl>
