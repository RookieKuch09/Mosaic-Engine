#pragma once

#include <type_traits>

#include <glm/glm.hpp>

namespace Mosaic
{
    template <typename _Type>
    requires std::is_arithmetic_v<_Type>
    class MeshScalar;

    template <typename>
    struct IsMeshScalarType : std::false_type
    {
    };

    template <typename _Type>
    struct IsMeshScalarType<MeshScalar<_Type>> : std::true_type
    {
    };

    template <auto _Semantic, typename _Type, std::size_t _Count>
    requires std::is_enum_v<decltype(_Semantic)> && IsMeshScalarType<_Type>::value
    class MeshAttribute;

    template <typename _AttributePack, typename _Flags>
    class MeshDescriptor;

    template <typename>
    struct IsMeshAttributeType : std::false_type
    {
    };

    template <auto _Semantic, typename _Type, std::size_t _Count>
    struct IsMeshAttributeType<MeshAttribute<_Semantic, _Type, _Count>> : std::true_type
    {
    };

    template <typename _Attribute>
    struct GetSemantic
    {
        static constexpr auto Value = _Attribute::SemanticValue;

        using Type = typename _Attribute::SemanticType;
    };

    template <auto...>
    struct AreAttributeSemanticsUnique : std::true_type
    {
    };

    template <auto _First, auto... _Rest>
    struct AreAttributeSemanticsUnique<_First, _Rest...> : std::bool_constant<((_First != _Rest) && ...) && AreAttributeSemanticsUnique<_Rest...>::value>
    {
    };

    template <typename... _Attributes>
    struct AreAllAttributeSemanticsValid
    {
        static constexpr bool value = AreAttributeSemanticsUnique<GetSemantic<_Attributes>::Value...>::value;
    };

    template <typename...>
    struct AreAttributeSemanticsSameType : std::true_type
    {
    };

    template <typename _First, typename _Second, typename... _Rest>
    struct AreAttributeSemanticsSameType<_First, _Second, _Rest...> : std::bool_constant<std::is_same_v<_First, _Second> && AreAttributeSemanticsSameType<_Second, _Rest...>::value>
    {
    };

    template <typename... _Attributes>
    struct AreAllAttributeSemanticsSameType
    {
        static constexpr bool value = AreAttributeSemanticsSameType<typename GetSemantic<_Attributes>::Type...>::value;
    };

    template <typename... _Attributes>
    concept MeshAttributesAreValid = AreAllAttributeSemanticsValid<_Attributes...>::value &&
                                     AreAllAttributeSemanticsSameType<_Attributes...>::value &&
                                     (IsMeshAttributeType<_Attributes>::value && ...);

    template <typename>
    struct IsMeshDescriptorType : std::false_type
    {
    };

    template <typename _AttributePack, typename _Flags>
    struct IsMeshDescriptorType<MeshDescriptor<_AttributePack, _Flags>> : std::true_type
    {
    };
}