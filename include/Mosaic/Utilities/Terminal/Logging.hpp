#pragma once

#include <format>

namespace Mosaic::Utilities::Terminal
{
    template <auto Severity, typename... Args>
    inline void Log(const std::format_string<Args...>& message, Args&&... args);

    template <typename... Args>
    inline void LogNotice(const std::format_string<Args...>& message, Args&&... args);

    template <typename... Args>
    inline void LogError(const std::format_string<Args...>& message, Args&&... args);

    template <typename... Args>
    inline void LogWarning(const std::format_string<Args...>& message, Args&&... args);

    template <typename... Args>
    inline void LogSuccess(const std::format_string<Args...>& message, Args&&... args);
}

#include <Mosaic/Inline/Utilities/Terminal/Logging.inl>