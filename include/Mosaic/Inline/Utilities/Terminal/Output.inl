#pragma once

#include <Mosaic/Utilities/Terminal/Output.hpp>

#include <iostream>

namespace Mosaic::Utilities::Terminal
{
    template <typename... Args>
    void Print(const std::format_string<Args...>& message, Args&&... args)
    {
        std::cout << std::format(message, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void PrintLine(const std::format_string<Args...>& message, Args&&... args)
    {
        std::cout << std::format(message, std::forward<Args>(args)...) << std::endl;
    }
}