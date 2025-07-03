#pragma once

#include <Mosaic/Debug/Console/Handle.hpp>
#include <Mosaic/Debug/Console/Severity.hpp>

#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include <magic_enum/magic_enum.hpp>

namespace Mosaic
{
    class Application;

    class ConsoleFileOutput
    {
    private:
        std::ofstream mStream;
        std::string mFilepath;

        friend class Console;
    };

    using ConsoleEngineLogNotice = ConsoleSeverityLevel<ConsoleEngineLogSeverity::ENGINE_NOTICE, ConsoleTextColour::Blue, ConsoleTextStyle::Bold>;
    using ConsoleEngineLogSuccess = ConsoleSeverityLevel<ConsoleEngineLogSeverity::ENGINE_SUCCESS, ConsoleTextColour::Green, ConsoleTextStyle::Bold>;
    using ConsoleEngineLogWarning = ConsoleSeverityLevel<ConsoleEngineLogSeverity::ENGINE_WARNING, ConsoleTextColour::Yellow, ConsoleTextStyle::Bold>;
    using ConsoleEngineLogError = ConsoleSeverityLevel<ConsoleEngineLogSeverity::ENGINE_ERROR, ConsoleTextColour::Red, ConsoleTextStyle::Bold>;

    class Console
    {
    private:
        template <typename _Severity>
        requires IsConsoleSeverityLevelType<_Severity>
        static consteval auto GetLogPrefixStyled()
        {
            constexpr auto SemanticString = magic_enum::enum_name(_Severity::SemanticValue);
            constexpr auto Flags = _Severity::ANSIPrefix;
            constexpr auto Reset = ToArray("\033[0m");

            constexpr auto OpenBracket = ToArray("[");
            constexpr auto CloseBracket = ToArray("]:");

            constexpr auto Semantic = ToArray<SemanticString.size()>(SemanticString);

            return ConcatArrays(Flags, OpenBracket, Semantic, CloseBracket, Reset);
        }

        template <typename _Severity>
        requires IsConsoleSeverityLevelType<_Severity>
        static consteval auto GetLogPrefixUnstyled()
        {
            constexpr auto SemanticString = magic_enum::enum_name(_Severity::SemanticValue);

            constexpr auto OpenBracket = ToArray("[");
            constexpr auto CloseBracket = ToArray("]:");

            constexpr auto Semantic = ToArray<SemanticString.size()>(SemanticString);

            return ConcatArrays(OpenBracket, Semantic, CloseBracket);
        }

    public:
        [[nodiscard]] ConsoleOutputHandle CreateOutput(const std::string& filepath);

        template <typename _Severity, typename... _Args>
        requires IsConsoleSeverityLevelType<_Severity>
        static void Log(const std::format_string<_Args...>& message, _Args&&... args)
        {
            constexpr auto Prefix = GetLogPrefixStyled<_Severity>();

            std::cout << Prefix.data() << " " << std::format(message, std::forward<_Args>(args)...) << '\n';
        }

        template <typename _Severity, typename... _Args>
        requires IsConsoleSeverityLevelType<_Severity>
        void Log(ConsoleOutputHandle outputHandle, const std::format_string<_Args...>& message, _Args&&... args)
        {
            constexpr auto Prefix = GetLogPrefixUnstyled<_Severity>();

            if (not mFileOutputs.contains(outputHandle))
            {
                Log<ConsoleEngineLogWarning>("ConsoleOutputHandle {} does not exist. Log will be redirected to console", outputHandle);

                Log<_Severity>(message, args...);
            }
            else
            {
                auto& output = mFileOutputs[outputHandle];

                output.mStream << Prefix.data() << " " << std::format(message, std::forward<_Args>(args)...) << '\n';
                output.mStream.flush();
            }
        }

        template <typename... _Args>
        [[noreturn]] static void Halt(const std::format_string<_Args...>& message, _Args&&... args)
        {
            std::cout << "\033[31m\033[1m[ENGINE_HALT]: \033[0m" << std::format(message, std::forward<_Args>(args)...) << '\n';

            std::exit(1); // TODO: safely exit program
        }

        template <typename... _Args>
        [[noreturn]] void Halt(ConsoleOutputHandle outputHandle, const std::format_string<_Args...>& message, _Args&&... args)
        {
            if (not mFileOutputs.contains(outputHandle))
            {
                Log<ConsoleEngineLogWarning>("ConsoleOutputHandle {} does not exist. Halt message will be redirected to console", outputHandle);

                Halt(message, args...);
            }
            else
            {
                auto& output = mFileOutputs[outputHandle];

                output.mStream << "[ENGINE_HALT]: " << std::format(message, std::forward<_Args>(args)...) << '\n';
                output.mStream.flush();

                std::exit(1); // TODO: safely exit program
            }
        }

        template <typename... _Args>
        [[noreturn]] static void Halt(std::int32_t exitCode, const std::format_string<_Args...>& message, _Args&&... args)
        {
            std::cout << "\033[31m\033[1m[ENGINE HALT]: \033[0m" << std::format(message, std::forward<_Args>(args)...) << '\n';

            std::exit(exitCode); // TODO: safely exit program
        }

        template <typename... _Args>
        [[noreturn]] void Halt(std::int32_t exitCode, ConsoleOutputHandle outputHandle, const std::format_string<_Args...>& message, _Args&&... args)
        {
            if (not mFileOutputs.contains(outputHandle))
            {
                Log<ConsoleEngineLogWarning>("ConsoleOutputHandle {} does not exist. Halt message will be redirected to console", outputHandle);

                Halt(exitCode, message, args...);
            }
            else
            {
                auto& output = mFileOutputs[outputHandle];

                output.mStream << "[ENGINE_HALT]: " << std::format(message, std::forward<_Args>(args)...) << '\n';
                output.mStream.flush();

                std::exit(exitCode); // TODO: safely exit program
            }
        }

    private:
        Console();

        std::unordered_map<ConsoleOutputHandle, ConsoleFileOutput> mFileOutputs;
        std::unordered_map<std::string, ConsoleOutputHandle> mFilepathHandles;

        ConsoleOutputHandle mNextAvailableOutputHandle;

        template <typename T> requires std::is_base_of_v<Application, T>
        friend class Instance;
    };
}