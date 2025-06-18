#pragma once

#include <mosaic/debug/console.hpp>

namespace Mosaic
{
    template <Console::LogSeverity Severity, typename... Args>
    void Console::Log(OutputID outputID, const std::format_string<Args...>& message, Args&&... args)
    {
        std::string prefix = GetPrefix<Severity>();
        std::string formatted = std::format(message, std::forward<Args>(args)...);
        std::string timestamp = "[" + GetTimestamp() + "]";

        if (outputID == TerminalOutputID)
        {
            DispatchToTerminal(timestamp + prefix + formatted);
        }
        else if (mFileOutputs.contains(outputID))
        {
            DispatchToFile(mFileOutputs[outputID], timestamp + prefix + formatted);
        }
        else
        {
            auto warning = std::format("OutputID {} does not exist or is unavailable. Rerouting output to terminal:", outputID);

            DispatchToTerminal(timestamp + " [Reroute] " + warning);
            DispatchToTerminal(timestamp + prefix + formatted);
        }
    }

    template <Console::LogSeverity Severity, typename... Args>
    void Console::Log(const std::format_string<Args...>& message, Args&&... args)
    {
        std::string prefix = GetPrefix<Severity>();
        std::string formatted = std::format(message, std::forward<Args>(args)...);
        std::string timestamp = "[" + GetTimestamp() + "]";

        DispatchToTerminal(timestamp + prefix + formatted);
    }

    template <typename... Args>
    void Console::Throw(const std::format_string<Args...>& message, Args&&... args)
    {
        std::string formatted = std::format(message, std::forward<Args>(args)...);
        std::string timestamp = "[" + GetTimestamp() + "]";

        DispatchToTerminal(timestamp + " [Throw] " + formatted);

        std::exit(1);
    }

    template <typename... Args>
    void Console::Throw(OutputID outputID, const std::format_string<Args...>& message, Args&&... args)
    {
        std::string formatted = std::format(message, std::forward<Args>(args)...);
        std::string timestamp = "[" + GetTimestamp() + "]";

        if (outputID == TerminalOutputID)
        {
            DispatchToTerminal(timestamp + " [Throw] " + formatted);
        }
        else if (auto file = mFileOutputs.find(outputID); file != mFileOutputs.end())
        {
            DispatchToFile(file->second, timestamp + " [Throw] " + formatted);
        }
        else
        {
            auto warning = std::format("OutputID {} does not exist or is unavailable. Rerouting output to terminal:", outputID);

            DispatchToTerminal(timestamp + " [Reroute] " + warning);
            DispatchToTerminal(timestamp + " [Throw] " + formatted);
        }

        std::exit(1);
    }

    template <Console::LogSeverity Severity>
    constexpr auto Console::GetPrefix() -> std::string
    {
        switch (Severity)
        {
            case LogSeverity::Success:
            {
                return " [Success] ";
            }
            case LogSeverity::Notice:
            {
                return " [Notice] ";
            }
            case LogSeverity::Warning:
            {
                return " [Warning] ";
            }
            case LogSeverity::Error:
            {
                return " [Error] ";
            }
            case LogSeverity::Fatal:
            {
                return " [Fatal] ";
            }
        }

        return " [Unknown] ";
    }
}