#pragma once

#include <array>
#include <string_view>
#include <type_traits>

namespace Mosaic
{
    enum class ConsoleTextModifier
    {
        Reset,
        Red,
        Green,
        Yellow,
        Blue,
        Magenta,
        Cyan,
        White,
        Black,
        Bold,
        Italic,
    };

    template <ConsoleTextModifier Modifier>
    static consteval auto ModifierToANSI()
    {
        constexpr auto code = []() -> std::string_view
        {
            switch (Modifier)
            {
                case ConsoleTextModifier::Reset:
                    return "\033[0m";
                case ConsoleTextModifier::Red:
                    return "\033[31m";
                case ConsoleTextModifier::Green:
                    return "\033[32m";
                case ConsoleTextModifier::Yellow:
                    return "\033[33m";
                case ConsoleTextModifier::Blue:
                    return "\033[34m";
                case ConsoleTextModifier::Magenta:
                    return "\033[35m";
                case ConsoleTextModifier::Cyan:
                    return "\033[36m";
                case ConsoleTextModifier::White:
                    return "\033[37m";
                case ConsoleTextModifier::Black:
                    return "\033[30m";
                case ConsoleTextModifier::Bold:
                    return "\033[1m";
                case ConsoleTextModifier::Italic:
                    return "\033[3m";
                default:
                    return "";
            }
        }();

        std::array<char, code.size()> result{};

        for (size_t i = 0; i < code.size(); ++i)
        {
            result[i] = code[i];
        }

        return result;
    }

    template <ConsoleTextModifier... Modifiers>
    static consteval auto BuildANSIPrefix()
    {
        constexpr std::size_t totalSize = (ModifierToANSI<Modifiers>().size() + ... + 0);
        std::array<char, totalSize> buffer{};

        std::size_t offset = 0;

        // Fold over each modifier, copying its characters into the buffer
        auto copyModifier = [&](auto part)
        {
            for (char c : part)
            {
                buffer[offset++] = c;
            }
        };

        (copyModifier(ModifierToANSI<Modifiers>()), ...);

        return buffer;
    }

    template <auto _Semantic, ConsoleTextModifier... _Modifiers>
    requires std::is_enum_v<decltype(_Semantic)>
    class ConsoleSeverityLevel
    {
    public:
        ConsoleSeverityLevel() = delete;

        using SemanticType = decltype(_Semantic);

        static constexpr SemanticType SemanticValue = _Semantic;
        static constexpr auto ANSIPrefix = BuildANSIPrefix<_Modifiers...>();
    };

    template <typename>
    inline constexpr bool IsConsoleSeverityLevelType = false;

    template <auto _Semantic, ConsoleTextModifier... _Modifiers>
    inline constexpr bool IsConsoleSeverityLevelType<ConsoleSeverityLevel<_Semantic, _Modifiers...>> = true;
}