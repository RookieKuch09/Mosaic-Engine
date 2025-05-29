#pragma once

#include <chrono>
#include <iostream>
#include <string>

namespace Mosaic::Utilities
{
    template <typename... Args>
    inline void Assert(bool condition, const std::string& message, Args&&... args)
    {
        if (condition)
        {
            Throw(message, std::forward<Args>(args)...);
        }
    }

    template <typename... Args>
    inline void LogSuccess(const std::string& message, Args&&... args)
    {
        auto now = std::chrono::system_clock::now();
        std::time_t nowClock = std::chrono::system_clock::to_time_t(now);
        std::tm localTime = *std::localtime(&nowClock);
        std::string time = std::format("[{:02}:{:02}:{:02}]", localTime.tm_hour, localTime.tm_min, localTime.tm_sec);
        std::string content = std::vformat(message, std::make_format_args(args...));

        std::cout << std::vformat("\033[32m{} (SUCCESS): \033[0m{}\n", std::make_format_args(time, content));
    }

    template <typename... Args>
    inline void LogNotice(const std::string& message, Args&&... args)
    {
        auto now = std::chrono::system_clock::now();
        std::time_t nowClock = std::chrono::system_clock::to_time_t(now);
        std::tm localTime = *std::localtime(&nowClock);
        std::string time = std::format("[{:02}:{:02}:{:02}]", localTime.tm_hour, localTime.tm_min, localTime.tm_sec);
        std::string content = std::vformat(message, std::make_format_args(args...));

        std::cout << std::vformat("\033[34m{} (NOTICE): \033[0m{}\n", std::make_format_args(time, content));
    }

    template <typename... Args>
    inline void LogWarning(const std::string& message, Args&&... args)
    {
        auto now = std::chrono::system_clock::now();
        std::time_t nowClock = std::chrono::system_clock::to_time_t(now);
        std::tm localTime = *std::localtime(&nowClock);
        std::string time = std::format("[{:02}:{:02}:{:02}]", localTime.tm_hour, localTime.tm_min, localTime.tm_sec);
        std::string content = std::vformat(message, std::make_format_args(args...));

        std::cout << std::vformat("\033[33m{} (WARNING): \033[0m{}\n", std::make_format_args(time, content));
    }

    template <typename... Args>
    inline void LogError(const std::string& message, Args&&... args)
    {
        auto now = std::chrono::system_clock::now();
        std::time_t nowClock = std::chrono::system_clock::to_time_t(now);
        std::tm localTime = *std::localtime(&nowClock);
        std::string time = std::format("[{:02}:{:02}:{:02}]", localTime.tm_hour, localTime.tm_min, localTime.tm_sec);
        std::string content = std::vformat(message, std::make_format_args(args...));

        std::cout << std::vformat("\033[31m{} (ERROR): \033[0m{}\n", std::make_format_args(time, content));
    }

    template <typename... Args>
    inline void Throw(const std::string& message, Args&&... args)
    {
        std::string content = std::vformat(message, std::make_format_args(args...));

        throw std::runtime_error(content);
    }
}
