#pragma once

#include <chrono>
#include <format>
#include <iostream>
#include <string>

namespace Mosaic::Utilities
{
    inline std::string CurrentTimestamp()
    {
        auto now = std::chrono::system_clock::now();
        std::time_t nowClock = std::chrono::system_clock::to_time_t(now);
        std::tm localTime = *std::localtime(&nowClock);
        return std::format("[{:02}:{:02}:{:02}]", localTime.tm_hour, localTime.tm_min, localTime.tm_sec);
    }

    template <typename... Args>
    inline void Assert(bool condition, std::format_string<Args...> message, Args&&... args)
    {
        if (condition)
        {
            Throw(message, std::forward<Args>(args)...);
        }
    }

    template <typename... Args>
    inline void LogSuccess(std::format_string<Args...> message, Args&&... args)
    {
        std::cout << std::format("\033[32m{} (SUCCESS): \033[0m{}\n",
                                 CurrentTimestamp(),
                                 std::format(message, std::forward<Args>(args)...));
    }

    template <typename... Args>
    inline void LogNotice(std::format_string<Args...> message, Args&&... args)
    {
        std::cout << std::format("\033[34m{} (NOTICE): \033[0m{}\n",
                                 CurrentTimestamp(),
                                 std::format(message, std::forward<Args>(args)...));
    }

    template <typename... Args>
    inline void LogWarning(std::format_string<Args...> message, Args&&... args)
    {
        std::cout << std::format("\033[33m{} (WARNING): \033[0m{}\n",
                                 CurrentTimestamp(),
                                 std::format(message, std::forward<Args>(args)...));
    }

    template <typename... Args>
    inline void LogError(std::format_string<Args...> message, Args&&... args)
    {
        std::cout << std::format("\033[31m{} (ERROR): \033[0m{}\n",
                                 CurrentTimestamp(),
                                 std::format(message, std::forward<Args>(args)...));
    }

    template <typename... Args>
    [[noreturn]] inline void Throw(std::format_string<Args...> message, Args&&... args)
    {
        throw std::runtime_error(std::format(message, std::forward<Args>(args)...));
    }
}
