#pragma once

#include <type_traits>

#include <glm/glm.hpp>

namespace Mosaic
{
    enum class MeshAttributeScalarType
    {
        I8,
        I16,
        I32,
        I64,
        U8,
        U16,
        U32,
        U64,
        F32,
        F64,
    };

    template <typename>
    class MeshAttributeScalarInfo;

    template <typename _Type>
    requires std::is_arithmetic_v<_Type>
    class MeshAttributeScalarInfo<_Type>
    {
    public:
        MeshAttributeScalarInfo() = delete;

        using Type = _Type;

        static constexpr std::size_t Count = 1;
    };

    template <glm::length_t _Count, typename _Type, glm::qualifier _Qualifier>
    requires std::is_arithmetic_v<_Type> && (_Count > 0)
    class MeshAttributeScalarInfo<glm::vec<_Count, _Type, _Qualifier>>
    {
    public:
        MeshAttributeScalarInfo() = delete;

        using Type = _Type;

        static constexpr std::size_t Count = _Count;
    };

    template <typename>
    inline constexpr bool IsMeshAttributeScalarInfoType = false;

    template <typename _Type>
    inline constexpr bool IsMeshAttributeScalarInfoType<MeshAttributeScalarInfo<_Type>> = true;

    template <typename T, typename = void>
    inline constexpr bool IsMeshAttributeScalarInfoCompatible = false;

    template <typename T>
    inline constexpr bool IsMeshAttributeScalarInfoCompatible<T, std::void_t<decltype(MeshAttributeScalarInfo<T>::Count), typename MeshAttributeScalarInfo<T>::Type>> = true;
}