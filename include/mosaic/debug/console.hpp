#pragma once

#include <mosaic/api/exposure.hpp>

#include <cstdint>
#include <format>
#include <fstream>
#include <string>
#include <unordered_map>

namespace Mosaic
{
    struct Resources;

    namespace Debug
    {
        class MOSAIC_PUBLIC_EXPOSURE Console
        {
        public:
            using OutputID = std::uint32_t;

            static constexpr OutputID TerminalOutputID = 0;

            enum class OutputType : std::uint8_t
            {
                Terminal,
                File,
            };

            enum class LogSeverity : std::uint8_t
            {
                Success,
                Notice,
                Warning,
                Error,
                Fatal,
            };

            [[nodiscard]] auto CreateFileOutput(const std::string& filepath) -> OutputID;

            template <LogSeverity Severity, typename... Args>
            void Log(OutputID outputID, const std::format_string<Args...>& message, Args&&... args);

            template <LogSeverity Severity, typename... Args>
            void Log(const std::format_string<Args...>& message, Args&&... args);

            template <typename... Args>
            [[noreturn]] void Throw(const std::format_string<Args...>& message, Args&&... args);

            template <typename... Args>
            [[noreturn]] void Throw(OutputID outputID, const std::format_string<Args...>& message, Args&&... args);

        private:
            Console();

            struct FileOutput
            {
                std::ofstream Stream;
                std::string Filepath;
            };

            std::unordered_map<OutputID, FileOutput> mFileOutputs;
            std::unordered_map<std::string, OutputID> mFilepathIDs;

            OutputID mNextAvailableID;

            template <LogSeverity Severity>
            constexpr auto GetPrefix() -> std::string;

            static void DispatchToTerminal(const std::string& message);
            static void DispatchToFile(FileOutput& fileOutput, const std::string& message);

            static auto GetTimestamp() -> std::string;

            static void AddInitialLogstamp(std::ofstream& file, const std::string& filepath);

            friend struct Mosaic::Resources;
        };
    }
}

#include <mosaic/inline/debug/console.inl>