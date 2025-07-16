#pragma once

#include <Mosaic/Utilities/Terminal/Logging.hpp>

#include <iostream>

#include <magic_enum/magic_enum.hpp>

namespace Mosaic::Utilities::Terminal
{
    template <auto Severity, typename... Args>
    void Log(const std::format_string<Args...>& message, Args&&... args)
    {
        std::cout << magic_enum::enum_name(Severity) << ": " << std::format(message, std::forward<Args>(args)...) << std::endl;
    }

    template <typename... Args>
    void LogNotice(const std::format_string<Args...>& message, Args&&... args)
    {
        std::cout << "NOTICE: " << std::format(message, std::forward<Args>(args)...) << std::endl;
    }

    template <typename... Args>
    void LogError(const std::format_string<Args...>& message, Args&&... args)
    {
        std::cout << "ERROR: " << std::format(message, std::forward<Args>(args)...) << std::endl;
    }

    template <typename... Args>
    void LogWarning(const std::format_string<Args...>& message, Args&&... args)
    {
        std::cout << "WARNING: " << std::format(message, std::forward<Args>(args)...) << std::endl;
    }

    template <typename... Args>
    void LogSuccess(const std::format_string<Args...>& message, Args&&... args)
    {
        std::cout << "SUCCESS: " << std::format(message, std::forward<Args>(args)...) << std::endl;
    }
}