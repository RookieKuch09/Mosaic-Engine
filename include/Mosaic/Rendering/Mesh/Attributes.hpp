#pragma once

#include <Mosaic/Rendering/Mesh/Scalar.hpp>

#include <string_view>

#include <magic_enum/magic_enum.hpp>

namespace Mosaic
{
    struct MeshAttributeReflection
    {
        std::string_view Semantic;

        std::size_t ElementCount;
        std::size_t ElementSize;

        std::size_t ElementScalarCount;
        std::size_t ElementScalarSize;

        MeshAttributeScalarType ElementScalarType;
    };

    template <auto _Semantic, typename _Type, std::size_t _Count = 1>
    requires std::is_enum_v<decltype(_Semantic)> && IsMeshAttributeScalarInfoCompatible<_Type> && (_Count > 0) && std::is_arithmetic_v<typename MeshAttributeScalarInfo<_Type>::Type>
    class MeshAttribute
    {
    public:
        using ElementScalarType = typename MeshAttributeScalarInfo<_Type>::Type;

    private:
        static consteval MeshAttributeScalarType ElementScalarTypeEnum()
        {
            if constexpr (std::is_same_v<ElementScalarType, std::int8_t>)
            {
                return MeshAttributeScalarType::I8;
            }
            else if constexpr (std::is_same_v<ElementScalarType, std::int16_t>)
            {
                return MeshAttributeScalarType::I16;
            }
            else if constexpr (std::is_same_v<ElementScalarType, std::int32_t>)
            {
                return MeshAttributeScalarType::I32;
            }
            else if constexpr (std::is_same_v<ElementScalarType, std::int64_t>)
            {
                return MeshAttributeScalarType::I64;
            }
            else if constexpr (std::is_same_v<ElementScalarType, std::uint8_t>)
            {
                return MeshAttributeScalarType::U8;
            }
            else if constexpr (std::is_same_v<ElementScalarType, std::uint16_t>)
            {
                return MeshAttributeScalarType::U16;
            }
            else if constexpr (std::is_same_v<ElementScalarType, std::uint32_t>)
            {
                return MeshAttributeScalarType::U32;
            }
            else if constexpr (std::is_same_v<ElementScalarType, std::uint64_t>)
            {
                return MeshAttributeScalarType::U64;
            }
            else if constexpr (std::is_same_v<ElementScalarType, float>)
            {
                return MeshAttributeScalarType::F32;
            }
            else if constexpr (std::is_same_v<ElementScalarType, double>)
            {
                return MeshAttributeScalarType::F64;
            }
        }

    public:
        MeshAttribute() = delete;

        using SemanticType = decltype(_Semantic);

        using ElementType = _Type;

        static constexpr std::size_t ElementCount = _Count;
        static constexpr std::size_t ElementSize = sizeof(ElementType);

        static constexpr std::size_t ElementScalarCount = MeshAttributeScalarInfo<_Type>::Count;
        static constexpr std::size_t ElementScalarSize = sizeof(ElementScalarType);

        static constexpr SemanticType SemanticValue = _Semantic;

        static consteval MeshAttributeReflection Reflect()
        {
            return MeshAttributeReflection{
                .Semantic = magic_enum::enum_name(SemanticValue),
                .ElementCount = ElementCount,
                .ElementSize = ElementSize,
                .ElementScalarCount = ElementScalarCount,
                .ElementScalarSize = ElementScalarSize,
                .ElementScalarType = ElementScalarTypeEnum(),
            };
        }
    };

    template <typename>
    inline constexpr bool IsMeshAttributeType = false;

    template <auto _Semantic, typename _Type, std::size_t _Count>
    inline constexpr bool IsMeshAttributeType<MeshAttribute<_Semantic, _Type, _Count>> = true;

    template <typename _Attribute>
    requires IsMeshAttributeType<_Attribute>
    struct MeshAttributeTraits
    {
        MeshAttributeTraits() = delete;

        using Type = typename _Attribute::SemanticType;

        static constexpr auto Value = _Attribute::SemanticValue;
    };

    template <auto...>
    inline constexpr bool MeshAttributeSemanticsAreUnique = true;

    template <auto _First, auto... _Rest>
    inline constexpr bool MeshAttributeSemanticsAreUnique<_First, _Rest...> = std::bool_constant<((_First != _Rest) && ...) && MeshAttributeSemanticsAreUnique<_Rest...>>::value;

    template <typename _First, typename... _Rest>
    constexpr bool MeshAttributeSemanticsAreSameType = (std::is_same_v<_First, _Rest> && ...);

    template <typename... _Attributes>
    constexpr bool MeshAttributesAreAllValid = (IsMeshAttributeType<_Attributes> && ...);

    template <typename... _Attributes>
    requires MeshAttributeSemanticsAreUnique<MeshAttributeTraits<_Attributes>::Value...> &&
             MeshAttributeSemanticsAreSameType<typename MeshAttributeTraits<_Attributes>::Type...> &&
             MeshAttributesAreAllValid<_Attributes...>
    class MeshAttributePack
    {
    private:
        template <typename _Attribute>
        static consteval auto GetAttributeSemantic()
        {
            return _Attribute::SemanticValue;
        }

    public:
        MeshAttributePack() = delete;

        template <std::size_t N>
        using GetAttributeByIndex = typename std::tuple_element<N, std::tuple<_Attributes...>>::type;

        template <auto _Semantic>
        static constexpr bool HasAttribute = ((GetAttributeSemantic<_Attributes>() == _Semantic) || ...);

        static constexpr std::size_t AttributeCount = sizeof...(_Attributes);

        static constexpr std::size_t VertexSize = ((_Attributes::ElementCount * _Attributes::ElementSize) + ...);

        template <typename _Function>
        static consteval void ForEach(_Function&& function)
        {
            (function.template operator()<_Attributes>(), ...);
        }
    };

    template <typename>
    inline constexpr bool IsMeshAttributePackType = false;

    template <typename... _Attributes>
    inline constexpr bool IsMeshAttributePackType<MeshAttributePack<_Attributes...>> = true;
}