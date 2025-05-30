#pragma once

#include <string>

namespace Mosaic::Systems::Internal
{
    class Console
    {
    public:
        template <typename... Args>
        static void Assert(bool condition, const std::string& message, Args&&... args);

        template <typename... Args>
        static void LogSuccess(const std::string& message, Args&&... args);

        template <typename... Args>
        static void LogNotice(const std::string& message, Args&&... args);

        template <typename... Args>
        static void LogWarning(const std::string& message, Args&&... args);

        template <typename... Args>
        static void LogError(const std::string& message, Args&&... args);

        template <typename... Args>
        static void Throw(const std::string& message, Args&&... args);

    private:
        static std::string GetCurrentTime();
    };
}

#include "application/console.inl"
