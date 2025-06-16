#pragma once

#include <mosaic/api/exposure.hpp>

#include <cstdint>
#include <format>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

namespace Mosaic
{
    struct Resources;

    class MOSAIC_PUBLIC_EXPOSURE Console
    {
    public:
        using OutputID = std::uint32_t;

        static constexpr OutputID TerminalOutputID = 0;

        enum class OutputType
        {
            Terminal,
            File,
        };

        enum class LogSeverity
        {
            Success,
            Notice,
            Warning,
            Error,
            Fatal,
        };

        [[nodiscard]] OutputID CreateFileOutput(const std::string& filepath);

        template <LogSeverity Severity, typename... Args>
        inline void Log(OutputID outputID, const std::format_string<Args...>& message, Args&&... args)
        {
            std::string prefix;

            switch (Severity)
            {
                case LogSeverity::Success:
                {
                    prefix = "[Success] ";
                    break;
                }
                case LogSeverity::Notice:
                {
                    prefix = "[Notice] ";
                    break;
                }
                case LogSeverity::Warning:
                {
                    prefix = "[Warning] ";
                    break;
                }
                case LogSeverity::Error:
                {
                    prefix = "[Error] ";
                    break;
                }
                case LogSeverity::Fatal:
                {
                    prefix = "[Fatal] ";
                    break;
                }
            }

            std::string formatted = std::format(message, std::forward<Args>(args)...);

            std::string timestamp = "[" + GetTimestamp() + "] ";

            if (outputID == TerminalOutputID)
            {
                DispatchToTerminal(timestamp + prefix + formatted);
            }
            else if (auto it = mFileOutputs.find(outputID); it != mFileOutputs.end())
            {
                DispatchToFile(it->second, timestamp + prefix + formatted);
            }
            else
            {
                auto warning = std::format("OutputID {} does not exist or is unavailable. Rerouting output to terminal:", outputID);

                DispatchToTerminal(timestamp + "[Reroute] " + warning);
                DispatchToTerminal(timestamp + prefix + formatted);
            }
        }

    private:
        Console() = default;

        struct FileOutput
        {
            std::ofstream Stream;
            std::string Filepath;
        };

        std::unordered_map<OutputID, FileOutput> mFileOutputs;

        void DispatchToTerminal(const std::string& message);
        void DispatchToFile(FileOutput& fileOutput, const std::string& message);

        OutputID HashFilePath(const std::string& filepath);

        std::string GetTimestamp();

        void AddInitialLogstamp(std::ofstream& file, const std::string& filepath);

        friend struct Resources;
    };
}