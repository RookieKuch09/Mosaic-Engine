#pragma once

#include <Mosaic/Debug/Console.hpp>

#include <array>
#include <cstring>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Mosaic
{
    template <typename T>
    struct MeshAttributeTypeInfo;

    template <typename T> requires std::is_arithmetic_v<T>
    struct MeshAttributeTypeInfo<T>
    {
        using Type = T;

        static constexpr std::size_t Size = sizeof(T);
        static constexpr std::size_t Count = 1;
    };

    template <typename T> requires std::is_arithmetic_v<T>
    struct MeshAttributeTypeInfo<glm::vec<2, T>>
    {
        using Type = T;

        static constexpr std::size_t Size = sizeof(T);
        static constexpr std::size_t Count = 2;
    };

    template <typename T> requires std::is_arithmetic_v<T>
    struct MeshAttributeTypeInfo<glm::vec<3, T>>
    {
        using Type = T;

        static constexpr std::size_t Size = sizeof(T);
        static constexpr std::size_t Count = 3;
    };

    template <typename T> requires std::is_arithmetic_v<T>
    struct MeshAttributeTypeInfo<glm::vec<4, T>>
    {
        using Type = T;

        static constexpr std::size_t Size = sizeof(T);
        static constexpr std::size_t Count = 4;
    };

    template <auto S, typename T, std::size_t N = 1>
    struct MeshAttribute
    {
        using SemanticType = decltype(S);
        using ValueType = T;
        using TypeInfo = MeshAttributeTypeInfo<T>;

        static constexpr auto Semantic = S;
        static constexpr std::size_t Count = N;
    };

    template <typename... A>
    class MeshDescriptor
    {
    public:
        static constexpr std::size_t AttributeCount = sizeof...(A);
        static constexpr std::size_t Stride = ((A::TypeInfo::Size * A::TypeInfo::Count * A::Count) + ...);

        template <auto S>
        consteval static std::size_t GetOffset();

        template <auto S>
        consteval static std::size_t GetIndex();

        template <auto S>
        using Attribute = std::tuple_element_t<MeshDescriptor::template GetIndex<S>(), std::tuple<A...>>;

    private:
        template <auto S, std::size_t I = 0>
        consteval static std::size_t GetIndexHelper();

        static constexpr std::array<std::size_t, AttributeCount> Offsets = []
        {
            std::array<std::size_t, AttributeCount> arr{};

            std::size_t offset = 0;
            std::size_t i = 0;

            ((arr[i++] = std::exchange(offset, offset + (A::TypeInfo::Size * A::TypeInfo::Count * A::Count))), ...);

            return arr;
        }();
    };

    template <typename D, typename IT> requires std::is_integral_v<typename MeshAttributeTypeInfo<IT>::Type>
    class Mesh
    {
    public:
        using Descriptor = D;
        using IndexType = IT;

        Mesh()
            : mVertexCount(0)
        {
        }

        template <auto S, typename T>
        void SetAttributeData(const std::vector<T>& data)
        {
            using Attribute = typename Descriptor::template Attribute<S>;
            using ElementType = typename Attribute::TypeInfo::Type;

            static_assert(std::is_same_v<T, glm::vec<Attribute::TypeInfo::Count, ElementType>>, "Mesh::SetAttributeData: vector element type does not match attribute format");

            if (data.size() % Attribute::Count != 0)
            {
                throw EarlyExit(1, std::format("Mesh::SetAttributeData: attribute '{}' expects {} elements per vertex, but received {} elements", static_cast<int>(S), Attribute::Count, data.size()));
            }

            const std::size_t numVertices = data.size() / Attribute::Count;

            if (mVertexCount == 0)
            {
                mVertexCount = numVertices;
                mData.resize(mVertexCount * Descriptor::Stride);
            }
            else if (mVertexCount != numVertices)
            {
                throw EarlyExit(1, std::format("Mesh::SetAttributeData: mismatched vertex count. Expected {}, got {}", mVertexCount, numVertices));
            }

            const std::size_t stride = Descriptor::Stride;
            const std::size_t offset = Descriptor::template GetOffset<S>();

            for (std::size_t i = 0; i < mVertexCount; ++i)
            {
                const T& value = data[i];

                std::memcpy(mData.data() + (i * stride + offset), &value, sizeof(T));
            }
        }

        void SetIndexData(const std::vector<IT>& data)
        {
            using IndexTypeInfo = MeshAttributeTypeInfo<IT>;

            const std::size_t elementCount = IndexTypeInfo::Count * data.size();

            if (elementCount % 3 != 0)
            {
                throw EarlyExit(1, "Mesh::SetIndexData: element count of index data must be a multiple of 3");
            }

            for (const IT& idx : data)
            {
                if constexpr (std::is_integral_v<IT>)
                {
                    if (idx >= mVertexCount)
                    {
                        throw EarlyExit(1, std::format("Mesh::SetIndexData: index {} out of bounds for vertex count {}", idx, mVertexCount));
                    }
                }
                else if constexpr (glm::vec<3, typename MeshAttributeTypeInfo<IT>::Type>::length() == 3 and std::is_integral_v<typename MeshAttributeTypeInfo<IT>::Type>)
                {
                    for (int i = 0; i < 3; i++)
                    {
                        if (idx[i] >= mVertexCount)
                        {
                            throw EarlyExit(1, std::format("Mesh::SetIndexData: index {} (element {}) out of bounds for vertex count {}", glm::to_string(idx), idx[i], mVertexCount));
                        }
                    }
                }
                else
                {
                    static_assert("Mesh::SetIndexData: unsupported index type");
                }
            }

            mIndices.clear();

            mIndices = std::move(data);
        }

    private:
        std::vector<std::byte> mData;
        std::vector<IT> mIndices;
        std::size_t mVertexCount;
    };
}

#include <Mosaic/Inline/Renderer/Resources/Mesh.inl>