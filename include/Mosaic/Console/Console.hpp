#pragma once

#include <Mosaic/Macros/Symbols.hpp>

#include <format>
#include <string>
#include <string_view>

namespace Mosaic::Console
{
    static constexpr std::string_view TEXT_DEFAULT = "\033[0m";

    static constexpr std::string_view TEXT_BOLD_INTENSITY = "\033[1m";
    static constexpr std::string_view TEXT_NORMAL_INTENSITY = "\033[22m";
    static constexpr std::string_view TEXT_FAINT_INTENSITY = "\033[2m";

    static constexpr std::string_view TEXT_ITALIC_ON = "\033[3m";
    static constexpr std::string_view TEXT_UNDERLINE_ON = "\033[4m";
    static constexpr std::string_view TEXT_STRIKE_ON = "\033[9m";

    static constexpr std::string_view TEXT_ITALIC_OFF = "\033[23m";
    static constexpr std::string_view TEXT_UNDERLINE_OFF = "\033[24m";
    static constexpr std::string_view TEXT_STRIKE_OFF = "\033[29m";

    static constexpr std::string_view TEXT_DEFAULT_FOREGROUND = "\033[39m";
    static constexpr std::string_view TEXT_DEFAULT_BACKGROUND = "\033[49m";

    static constexpr std::string_view TEXT_FOREGROUND_BLACK = "\033[30m";
    static constexpr std::string_view TEXT_FOREGROUND_RED = "\033[31m";
    static constexpr std::string_view TEXT_FOREGROUND_GREEN = "\033[32m";
    static constexpr std::string_view TEXT_FOREGROUND_YELLOW = "\033[33m";
    static constexpr std::string_view TEXT_FOREGROUND_BLUE = "\033[34m";
    static constexpr std::string_view TEXT_FOREGROUND_MAGENTA = "\033[35m";
    static constexpr std::string_view TEXT_FOREGROUND_CYAN = "\033[36m";
    static constexpr std::string_view TEXT_FOREGROUND_WHITE = "\033[37m";

    static constexpr std::string_view TEXT_FOREGROUND_BRIGHT_BLACK = "\033[30;1m";
    static constexpr std::string_view TEXT_FOREGROUND_BRIGHT_RED = "\033[31;1m";
    static constexpr std::string_view TEXT_FOREGROUND_BRIGHT_GREEN = "\033[32;1m";
    static constexpr std::string_view TEXT_FOREGROUND_BRIGHT_YELLOW = "\033[33;1m";
    static constexpr std::string_view TEXT_FOREGROUND_BRIGHT_BLUE = "\033[34;1m";
    static constexpr std::string_view TEXT_FOREGROUND_BRIGHT_MAGENTA = "\033[35;1m";
    static constexpr std::string_view TEXT_FOREGROUND_BRIGHT_CYAN = "\033[36;1m";
    static constexpr std::string_view TEXT_FOREGROUND_BRIGHT_WHITE = "\033[37;1m";

    static constexpr std::string_view TEXT_BACKGROUND_BLACK = "\033[40m";
    static constexpr std::string_view TEXT_BACKGROUND_RED = "\033[41m";
    static constexpr std::string_view TEXT_BACKGROUND_GREEN = "\033[42m";
    static constexpr std::string_view TEXT_BACKGROUND_YELLOW = "\033[43m";
    static constexpr std::string_view TEXT_BACKGROUND_BLUE = "\033[44m";
    static constexpr std::string_view TEXT_BACKGROUND_MAGENTA = "\033[45m";
    static constexpr std::string_view TEXT_BACKGROUND_CYAN = "\033[46m";
    static constexpr std::string_view TEXT_BACKGROUND_WHITE = "\033[47m";

    static constexpr std::string_view TEXT_BACKGROUND_BRIGHT_BLACK = "\033[40;1m";
    static constexpr std::string_view TEXT_BACKGROUND_BRIGHT_RED = "\033[41;1m";
    static constexpr std::string_view TEXT_BACKGROUND_BRIGHT_GREEN = "\033[42;1m";
    static constexpr std::string_view TEXT_BACKGROUND_BRIGHT_YELLOW = "\033[43;1m";
    static constexpr std::string_view TEXT_BACKGROUND_BRIGHT_BLUE = "\033[44;1m";
    static constexpr std::string_view TEXT_BACKGROUND_BRIGHT_MAGENTA = "\033[45;1m";
    static constexpr std::string_view TEXT_BACKGROUND_BRIGHT_CYAN = "\033[46;1m";
    static constexpr std::string_view TEXT_BACKGROUND_BRIGHT_WHITE = "\033[47;1m";

    class MOSAIC_PUBLIC_SYMBOL Console
    {
    public:
        Console() = default;
        ~Console() = default;

        Console(const Console&) = delete;
        Console(Console&&) noexcept = delete;

        Console& operator=(const Console&) = delete;
        Console& operator=(Console&&) noexcept = delete;

        template <typename T>
        [[nodiscard]] inline std::optional<T> GetInput() const;

        template <typename... Args>
        inline void Print(std::format_string<Args...> message, Args&&... args) const;

        template <typename... Args>
        inline void PrintLine(std::format_string<Args...> message, Args&&... args) const;

        template <typename... Args>
        inline void LogNotice(std::format_string<Args...> message, Args&&... args) const;

        template <typename... Args>
        inline void LogError(std::format_string<Args...> message, Args&&... args) const;

        template <typename... Args>
        inline void LogWarning(std::format_string<Args...> message, Args&&... args) const;

    private:
        [[nodiscard]] std::string Timestamp() const;
    };
}

#include <Mosaic/Inline/Console/Console.inl>