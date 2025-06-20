#pragma once

#include <mosaic/debug/console.hpp>

namespace Mosaic
{
    template <Console::LogSeverity Severity, typename... Args>
    void Console::Log(const std::format_string<Args...>& message, Args&&... args)
    {
        std::string formatted = std::format(message, std::forward<Args>(args)...);
        std::string outputMessage = GetFullPrefixForTerminalOutput<Severity>() + formatted;

        DispatchToTerminal(outputMessage);
    }

    template <Console::LogSeverity Severity, typename... Args>
    void Console::Log(OutputID outputID, const std::format_string<Args...>& message, Args&&... args)
    {
        std::string formatted = std::format(message, std::forward<Args>(args)...);

        if (mFileOutputs.contains(outputID))
        {
            std::string outputMessage = GetFullPrefixForFileOutput<Severity>() + formatted;

            DispatchToFile(mFileOutputs[outputID], outputMessage);
        }
        else
        {
            std::string redirectMessage = GetTerminalRedirect(outputID);
            std::string outputMessage = GetFullPrefixForTerminalOutput<Severity>() + formatted;

            DispatchToTerminal(redirectMessage);
            DispatchToTerminal(outputMessage);
        }
    }

    template <typename... Args>
    void Console::Halt(const std::format_string<Args...>& message, Args&&... args)
    {
        std::string formatted = std::format(message, std::forward<Args>(args)...);
        std::string haltMessage = GetFullPrefixForTerminalHalt() + formatted;

        DispatchToTerminal(haltMessage);

        std::exit(1);
    }

    template <typename... Args>
    void Console::Halt(OutputID outputID, const std::format_string<Args...>& message, Args&&... args)
    {
        std::string formatted = std::format(message, std::forward<Args>(args)...);

        if (mFileOutputs.contains(outputID))
        {
            std::string haltMessage = GetFullPrefixForFileHalt() + formatted;

            DispatchToFile(mFileOutputs[outputID], haltMessage);
        }
        else
        {
            std::string redirectMessage = GetTerminalRedirect(outputID);
            std::string haltMessage = GetFullPrefixForTerminalHalt() + formatted;

            DispatchToTerminal(redirectMessage);
            DispatchToTerminal(haltMessage);
        }

        std::exit(1);
    }

    template <typename... Args>
    void Console::Halt(std::int32_t exitCode, const std::format_string<Args...>& message, Args&&... args)
    {
        std::string formatted = std::format(message, std::forward<Args>(args)...);
        std::string haltMessage = GetFullPrefixForTerminalHalt() + formatted;

        DispatchToTerminal(haltMessage);

        std::exit(exitCode);
    }

    template <typename... Args>
    void Console::Halt(std::int32_t exitCode, OutputID outputID, const std::format_string<Args...>& message, Args&&... args)
    {
        std::string formatted = std::format(message, std::forward<Args>(args)...);

        if (mFileOutputs.contains(outputID))
        {
            std::string haltMessage = GetFullPrefixForFileHalt() + formatted;

            DispatchToFile(mFileOutputs[outputID], haltMessage);
        }
        else
        {
            std::string redirectMessage = GetTerminalRedirect(outputID);
            std::string haltMessage = GetFullPrefixForTerminalHalt() + formatted;

            DispatchToTerminal(redirectMessage);
            DispatchToTerminal(haltMessage);
        }

        std::exit(exitCode);
    }

    template <Console::LogSeverity Severity>
    constexpr std::string Console::GetSeverityPrefixForFileOutput()
    {
        switch (Severity)
        {
            case LogSeverity::Success:
            {
                return "[SUCCESS] --";
            }
            case LogSeverity::Notice:
            {
                return "[NOTICE] ---";
            }
            case LogSeverity::Warning:
            {
                return "[WARNING] --";
            }
            case LogSeverity::Error:
            {
                return "[ERROR] ----";
            }
        }

        return "[UNKNOWN] --";
    }

    template <Console::LogSeverity Severity>
    constexpr std::string Console::GetSeverityPrefixForTerminalOutput()
    {
        switch (Severity)
        {
            case LogSeverity::Success:
            {
                return std::format(" {}{}[SUCCESS] --> ", ANSI_GREEN, ANSI_BOLD);
            }
            case LogSeverity::Notice:
            {
                return std::format(" {}{}[NOTICE] ---> ", ANSI_BLUE, ANSI_BOLD);
            }
            case LogSeverity::Warning:
            {
                return std::format(" {}{}[WARNING] --> ", ANSI_YELLOW, ANSI_BOLD);
            }
            case LogSeverity::Error:
            {
                return std::format(" {}{}[ERROR] ----> ", ANSI_RED, ANSI_BOLD);
            }
        }

        return "[UNKNOWN] --";
    }

    template <Console::LogSeverity Severity>
    std::string Console::GetFullPrefixForFileOutput()
    {
        return GetTimestampForFileOutput() + GetSeverityPrefixForFileOutput<Severity>() + ANSI_RESET;
    }

    template <Console::LogSeverity Severity>
    std::string Console::GetFullPrefixForTerminalOutput()
    {
        return GetTimestampForTerminalOutput() + GetSeverityPrefixForTerminalOutput<Severity>() + ANSI_RESET;
    }
}