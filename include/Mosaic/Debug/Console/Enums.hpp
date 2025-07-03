#pragma once

#include <cstdint>

namespace Mosaic
{
    enum class ConsoleTextColour : std::uint8_t
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
    };

    enum class ConsoleTextStyle : std::uint8_t
    {
        Bold,
        Italic,
        Underline,
        Strikethrough,
    };

    enum class ConsoleEngineLogSeverity : std::uint8_t
    {
        ENGINE_NOTICE,
        ENGINE_SUCCESS,
        ENGINE_WARNING,
        ENGINE_ERROR,
    };
}