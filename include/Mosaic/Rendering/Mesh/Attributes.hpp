#pragma once

#include <Mosaic/Rendering/Mesh/Validation.hpp>

#include <tuple>

namespace Mosaic
{
    template <typename _Type>
    requires std::is_arithmetic_v<_Type>
    class MeshScalar
    {
    public:
        MeshScalar() = delete;
        ~MeshScalar() = delete;

        MeshScalar(const MeshScalar&) = delete;
        MeshScalar(MeshScalar&&) noexcept = delete;

        MeshScalar& operator=(const MeshScalar&) = delete;
        MeshScalar& operator=(MeshScalar&&) noexcept = delete;

        using Type = _Type;

        static constexpr std::size_t Count = 1;
    };

    template <typename _Type, std::size_t _Count>
    requires std::is_arithmetic_v<_Type> && (_Count > 0)
    class MeshScalar<glm::vec<_Count, _Type>>
    {
    public:
        MeshScalar() = delete;
        ~MeshScalar() = delete;

        MeshScalar(const MeshScalar&) = delete;
        MeshScalar(MeshScalar&&) noexcept = delete;

        MeshScalar& operator=(const MeshScalar&) = delete;
        MeshScalar& operator=(MeshScalar&&) noexcept = delete;

        using Type = _Type;

        static constexpr std::size_t Count = _Count;
    };

    template <auto _Semantic, typename _Type, std::size_t _Count>
    requires std::is_enum_v<decltype(_Semantic)> && IsMeshScalarType<_Type>::value
    class MeshAttribute
    {
    public:
        MeshAttribute() = delete;
        ~MeshAttribute() = delete;

        MeshAttribute(const MeshAttribute&) = delete;
        MeshAttribute(MeshAttribute&&) noexcept = delete;

        MeshAttribute& operator=(const MeshAttribute&) = delete;
        MeshAttribute& operator=(MeshAttribute&&) noexcept = delete;

        using SemanticType = decltype(_Semantic);
        using ElementType = _Type;

        static constexpr std::size_t ElementCount = _Count;
        static constexpr SemanticType SemanticValue = _Semantic;
    };

    template <typename... _Attributes>
    requires MeshAttributesAreValid<_Attributes...>
    class MeshAttributePack
    {
    public:
        MeshAttributePack() = delete;
        ~MeshAttributePack() = delete;

        MeshAttributePack(const MeshAttributePack&) = delete;
        MeshAttributePack(MeshAttributePack&&) noexcept = delete;

        MeshAttributePack& operator=(const MeshAttributePack&) = delete;
        MeshAttributePack& operator=(MeshAttributePack&&) noexcept = delete;

        template <std::size_t N>
        using GetAttributeByIndex = typename std::tuple_element<N, std::tuple<_Attributes...>>::type;

        template <auto _Semantic>
        using GetAttributeBySemantic = int; // TODO: retrieve mesh attribute by semantic

        template <auto _Semantic>
        static constexpr bool HasAttribute = ((GetAttributeSemantic<_Attributes>() == _Semantic) || ...);

    private:
        template <typename Attribute>
        static constexpr auto GetAttributeSemantic()
        {
            return Attribute::SemanticValue;
        }
    };
}