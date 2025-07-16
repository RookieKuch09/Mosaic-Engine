#pragma once

#include <format>

namespace Mosaic::Utilities::Terminal
{
    template <typename... Args>
    inline void Print(const std::format_string<Args...>& message, Args&&... args);

    template <typename... Args>
    inline void PrintLine(const std::format_string<Args...>& message, Args&&... args);
}

#include <Mosaic/Inline/Utilities/Terminal/Output.inl>