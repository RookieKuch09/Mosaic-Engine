#pragma once

#include <type_traits>

namespace Mosaic
{
    template <auto S> requires std::is_enum_v<decltype(S)>
    struct Enumerator
    {
        static constexpr auto Value = S;
        using Type = decltype(S);
    };
}