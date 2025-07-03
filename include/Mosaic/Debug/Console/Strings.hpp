#pragma once

#include <array>
#include <string_view>

#include <Mosaic/Debug/Console/Enums.hpp>

namespace Mosaic
{
    template <ConsoleTextColour _Colour>
    static consteval auto ColourToANSI()
    {
        constexpr auto code = []() -> std::string_view
        {
            switch (_Colour)
            {
                case ConsoleTextColour::Red:
                {
                    return "\033[31m";
                }
                case ConsoleTextColour::Green:
                {
                    return "\033[32m";
                }
                case ConsoleTextColour::Yellow:
                {
                    return "\033[33m";
                }
                case ConsoleTextColour::Blue:
                {
                    return "\033[34m";
                }
                case ConsoleTextColour::Magenta:
                {
                    return "\033[35m";
                }
                case ConsoleTextColour::Cyan:
                {
                    return "\033[36m";
                }
                case ConsoleTextColour::White:
                {
                    return "\033[37m";
                }
                case ConsoleTextColour::Black:
                {
                    return "\033[30m";
                }
            }
        }();

        std::array<char, code.size() + 1> result = {};

        for (std::size_t i = 0; i < code.size(); i++)
        {
            result[i] = code[i];
        }

        result[code.size()] = '\0';

        return result;
    }

    template <ConsoleTextStyle _Style>
    static consteval auto StyleToANSI()
    {
        constexpr auto code = []() -> std::string_view
        {
            switch (_Style)
            {
                case ConsoleTextStyle::Bold:
                {
                    return "\033[1m";
                }
                case ConsoleTextStyle::Italic:
                {
                    return "\033[3m";
                }
                case ConsoleTextStyle::Underline:
                {
                    return "\033[4m";
                }
                case ConsoleTextStyle::Strikethrough:
                {
                    return "\033[9m";
                }
            }
        }();

        std::array<char, code.size() + 1> result = {};

        for (std::size_t i = 0; i < code.size(); i++)
        {
            result[i] = code[i];
        }

        result[code.size()] = '\0';

        return result;
    }

    template <std::size_t N>
    static consteval auto ToArray(const char (&str)[N])
    {
        std::array<char, N> result = {};

        for (std::size_t i = 0; i < N; i++)
        {
            result[i] = str[i];
        }

        return result;
    }

    template <std::size_t N>
    static consteval auto ToArray(std::string_view in)
    {
        std::array<char, N + 1> result = {};

        for (std::size_t i = 0; i < N; i++)
        {
            result[i] = in[i];
        }

        result[N] = '\0';

        return result;
    }

    template <std::size_t N>
    static consteval auto ConcatArrays(const std::array<char, N>& a)
    {
        return a;
    }

    template <std::size_t N1, std::size_t N2, std::size_t... Ns>
    static consteval auto ConcatArrays(const std::array<char, N1>& a, const std::array<char, N2>& b, const std::array<char, Ns>&... rest)
    {
        constexpr std::size_t len1 = N1 - 1;
        constexpr std::size_t len2 = N2 - 1;

        std::array<char, len1 + len2 + 1> merged = {};

        for (std::size_t i = 0; i < len1; i++)
        {
            merged[i] = a[i];
        }

        for (std::size_t i = 0; i < len2; i++)
        {
            merged[len1 + i] = b[i];
        }

        merged[len1 + len2] = '\0';

        if constexpr (sizeof...(rest) == 0)
        {
            return merged;
        }
        else
        {
            return ConcatArrays(merged, rest...);
        }
    }

    template <ConsoleTextColour _Colour, ConsoleTextStyle... _Styles>
    static consteval auto BuildANSIPrefix()
    {
        constexpr auto Colour = ColourToANSI<_Colour>();

        if constexpr (sizeof...(_Styles) == 0)
        {
            return Colour;
        }
        else
        {
            return ConcatArrays(Colour, StyleToANSI<_Styles>()...);
        }
    }
}