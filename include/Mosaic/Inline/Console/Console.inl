#pragma once

#include <Mosaic/Console/Console.hpp>

#include <iostream>
#include <sstream>

namespace Mosaic::Console
{
    template <typename T>
    std::optional<T> Console::GetInput() const
    {
        std::string input;

        std::getline(std::cin, input);

        std::istringstream buffer(input);

        T output;

        buffer >> output;

        if (buffer.fail() || !buffer.eof())
        {
            return std::nullopt;
        }

        return output;
    }

    template <typename... Args>
    void Console::Print(std::format_string<Args...> message, Args&&... args) const
    {
        std::cout << std::format(message, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Console::PrintLine(std::format_string<Args...> message, Args&&... args) const
    {
        std::cout << std::format(message, std::forward<Args>(args)...) << std::endl;
    }

    template <typename... Args>
    void Console::LogNotice(std::format_string<Args...> message, Args&&... args) const
    {
        std::string formattedMessage = std::format(message, std::forward<Args>(args)...);

        std::cout << Timestamp() << TEXT_FOREGROUND_BLUE << " NOTICE: " << TEXT_DEFAULT << formattedMessage << std::endl;
    }

    template <typename... Args>
    void Console::LogError(std::format_string<Args...> message, Args&&... args) const
    {
        std::string formattedMessage = std::format(message, std::forward<Args>(args)...);

        std::cout << Timestamp() << TEXT_FOREGROUND_RED << " ERROR: " << TEXT_DEFAULT << formattedMessage << std::endl;
    }

    template <typename... Args>
    void Console::LogWarning(std::format_string<Args...> message, Args&&... args) const
    {
        std::string formattedMessage = std::format(message, std::forward<Args>(args)...);

        std::cout << Timestamp() << TEXT_FOREGROUND_YELLOW << " WARNING: " << TEXT_DEFAULT << formattedMessage << std::endl;
    }
}