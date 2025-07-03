#pragma once

#include <Mosaic/Debug/Console/Strings.hpp>

#include <type_traits>

namespace Mosaic
{

    template <ConsoleTextStyle...>
    inline constexpr bool ConsoleTextStylesAreUnique = true;

    template <ConsoleTextStyle _First, ConsoleTextStyle... _Rest>
    inline constexpr bool ConsoleTextStylesAreUnique<_First, _Rest...> = ((_First != _Rest) && ...) && ConsoleTextStylesAreUnique<_Rest...>;

    template <auto _Semantic, ConsoleTextColour _Colour, ConsoleTextStyle... _Styles>
    requires std::is_enum_v<decltype(_Semantic)> && ConsoleTextStylesAreUnique<_Styles...>
    class ConsoleSeverityLevel
    {
    public:
        using SemanticType = decltype(_Semantic);

        static constexpr SemanticType SemanticValue = _Semantic;
        static constexpr auto ANSIPrefix = BuildANSIPrefix<_Colour, _Styles...>();
    };

    template <typename>
    inline constexpr bool IsConsoleSeverityLevelType = false;

    template <auto _Semantic, ConsoleTextColour _Colour, ConsoleTextStyle... _Styles>
    inline constexpr bool IsConsoleSeverityLevelType<ConsoleSeverityLevel<_Semantic, _Colour, _Styles...>> = true;
}
