#pragma once

#include <Mosaic/Macros/Exposure.hpp>

#include <cstdint>
#include <format>
#include <fstream>
#include <string>
#include <unordered_map>

namespace Mosaic
{
    class Application;

    template <typename T> requires std::is_base_of_v<Application, T>
    class Instance;

    struct EarlyExit
    {
        std::uint32_t ExitCode = 0;
        std::string ExitMessage = "Runtime Exception";
    };

    using LogTargetID = std::uint32_t;

    enum class LogSeverity
    {
        Success,
        Notice,
        Warning,
        Error,
    };

    class MOSAIC_PUBLIC_EXPOSURE Console
    {
    public:
        [[nodiscard]] LogTargetID CreateLogTarget(const std::string& filepath);

        template <LogSeverity Severity, typename... Args>
        static void Log(const std::format_string<Args...>& message, Args&&... args);

        template <LogSeverity Severity, typename... Args>
        void Log(LogTargetID outputID, const std::format_string<Args...>& message, Args&&... args);

        template <typename... Args>
        [[noreturn]] static void Halt(const std::format_string<Args...>& message, Args&&... args);

        template <typename... Args>
        [[noreturn]] void Halt(LogTargetID outputID, const std::format_string<Args...>& message, Args&&... args);

        template <typename... Args>
        [[noreturn]] static void Halt(std::int32_t exitCode, const std::format_string<Args...>& message, Args&&... args);

        template <typename... Args>
        [[noreturn]] void Halt(std::int32_t exitCode, LogTargetID outputID, const std::format_string<Args...>& message, Args&&... args);

    private:
        static constexpr const char* ANSI_RESET = "\x1b[0m";
        static constexpr const char* ANSI_BOLD = "\x1b[1m";
        static constexpr const char* ANSI_DIM = "\x1b[2m";
        static constexpr const char* ANSI_ITALIC = "\x1b[3m";

        static constexpr const char* ANSI_RED = "\x1b[31m";
        static constexpr const char* ANSI_GREEN = "\x1b[32m";
        static constexpr const char* ANSI_YELLOW = "\x1b[33m";
        static constexpr const char* ANSI_BLUE = "\x1b[34m";
        static constexpr const char* ANSI_MAGENTA = "\x1b[35m";
        static constexpr const char* ANSI_CYAN = "\x1b[36m";
        static constexpr const char* ANSI_WHITE = "\x1b[37m";

        static constexpr const char* ANSI_BRIGHT_RED = "\x1b[91m";
        static constexpr const char* ANSI_BRIGHT_GREEN = "\x1b[92m";
        static constexpr const char* ANSI_BRIGHT_YELLOW = "\x1b[93m";
        static constexpr const char* ANSI_BRIGHT_BLUE = "\x1b[94m";
        static constexpr const char* ANSI_BRIGHT_MAGENTA = "\x1b[95m";
        static constexpr const char* ANSI_BRIGHT_CYAN = "\x1b[96m";

        Console();

        struct FileOutput
        {
            std::ofstream Stream;
            std::string Filepath;
        };

        std::unordered_map<LogTargetID, FileOutput> mFileOutputs;
        std::unordered_map<std::string, LogTargetID> mFilepathIDs;

        LogTargetID mNextAvailableID;

        template <LogSeverity Severity>
        static constexpr std::string GetSeverityPrefixForFileOutput();

        template <LogSeverity Severity>
        static constexpr std::string GetSeverityPrefixForTerminalOutput();

        template <LogSeverity Severity>
        static std::string GetFullPrefixForFileOutput();

        template <LogSeverity Severity>
        static std::string GetFullPrefixForTerminalOutput();

        static std::string GetFullPrefixForFileHalt();
        static std::string GetFullPrefixForTerminalHalt();

        std::string GetTerminalRedirect(LogTargetID outputID);

        static void DispatchToTerminal(const std::string& message);
        static void DispatchToFile(FileOutput& fileOutput, const std::string& message);

        static std::string GetTimestampForLogstamp();
        static std::string GetTimestampForFileOutput();
        static std::string GetTimestampForTerminalOutput();

        static void AddInitialLogstamp(std::ofstream& file, const std::string& filepath);

        template <typename T> requires std::is_base_of_v<Application, T>
        friend class Instance;
    };
}

#include <Mosaic/Inline/Debug/Console.inl>