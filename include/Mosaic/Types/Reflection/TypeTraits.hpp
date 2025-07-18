#pragma once

#include <type_traits>

namespace Mosaic::Types::Reflection
{
    template <typename T>
    struct TypeTraits
    {
        TypeTraits() = delete;

        static constexpr bool IsArithmetic = std::is_arithmetic_v<T>;
        static constexpr bool IsIntegral = std::is_integral_v<T>;
        static constexpr bool IsFloatingPoint = std::is_floating_point_v<T>;

        static constexpr bool IsDefaultConstructible = std::is_default_constructible_v<T>;
        static constexpr bool IsConstructible = std::is_constructible_v<T>;

        static constexpr bool IsVolatile = std::is_volatile_v<T>;
        static constexpr bool IsConst = std::is_const_v<T>;
        static constexpr bool IsPointer = std::is_pointer_v<T>;
        static constexpr bool IsReference = std::is_reference_v<T>;

        static constexpr bool IsClass = std::is_class_v<T>;
        static constexpr bool IsEnum = std::is_enum_v<T>;
        static constexpr bool IsUnion = std::is_union_v<T>;
        static constexpr bool IsFunction = std::is_function_v<T>;
        static constexpr bool IsArray = std::is_array_v<T>;

        static constexpr bool IsTrivial = std::is_trivial_v<T>;
        static constexpr bool IsTriviallyCopyable = std::is_trivially_copyable_v<T>;
        static constexpr bool IsTriviallyDestructible = std::is_trivially_destructible_v<T>;
        static constexpr bool IsTriviallyConstructible = std::is_trivially_constructible_v<T>;

        static constexpr bool IsTriviallyCopyAssignable = std::is_trivially_copy_assignable_v<T>;
        static constexpr bool IsTriviallyMoveAssignable = std::is_trivially_move_assignable_v<T>;

        static constexpr bool IsMoveConstructible = std::is_move_constructible_v<T>;
        static constexpr bool IsCopyConstructible = std::is_copy_constructible_v<T>;

        static constexpr bool IsDestructible = std::is_destructible_v<T>;

        static constexpr bool IsLValueReference = std::is_lvalue_reference_v<T>;
        static constexpr bool IsRValueReference = std::is_rvalue_reference_v<T>;

        using Type = T;
    };
}