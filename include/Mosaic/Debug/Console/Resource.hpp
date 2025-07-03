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

    struct ConsoleFileOutput
    {
        std::ofstream Stream;
        std::string Filepath;
    };

    class Console
    {
    private:
        template <std::size_t N1, std::size_t N2>
        static consteval auto ConcatArrays(const std::array<char, N1>& a, const std::array<char, N2>& b)
        {
            std::array<char, N1 + N2> result{};

            for (std::size_t i = 0; i < N1; i++)
            {
                result[i] = a[i];
            }

            for (std::size_t i = 0; i < N2; i++)
            {
                result[N1 + i] = b[i];
            }

            return result;
        }

        template <std::size_t N>
        static consteval auto ToArray(std::string_view sv)
        {
            std::array<char, N> array = {};

            for (std::size_t i = 0; i < N; i++)
            {
                array[i] = sv[i];
            }

            return array;
        }

        template <typename _Severity>
        requires IsConsoleSeverityLevelType<_Severity>
        static consteval auto GetLogPrefixStyled()
        {
            constexpr auto semantic_sv = magic_enum::enum_name(_Severity::SemanticValue);
            constexpr auto flags = _Severity::ANSIPrefix;
            constexpr auto reset = ToArray<4>("\033[0m");

            constexpr auto open_bracket = ToArray<1>("[");
            constexpr auto close_bracket_colon = ToArray<2>("]:");
            constexpr auto space = ToArray<1>(" ");

            // Convert semantic name to array
            constexpr auto semantic = ToArray<semantic_sv.size()>(semantic_sv);

            // Concatenate step by step
            constexpr auto part1 = ConcatArrays(flags, open_bracket);
            constexpr auto part2 = ConcatArrays(part1, semantic);
            constexpr auto part3 = ConcatArrays(part2, close_bracket_colon);
            constexpr auto part4 = ConcatArrays(part3, reset);
            constexpr auto result = ConcatArrays(part4, space);

            return result;
        }

        template <typename _Severity>
        requires IsConsoleSeverityLevelType<_Severity>
        static consteval auto GetLogPrefixUnstyled()
        {
            constexpr auto semantic_sv = magic_enum::enum_name(_Severity::SemanticValue);
            constexpr auto open_bracket = ToArray<1>("[");
            constexpr auto close_bracket_colon = ToArray<2>("]:");
            constexpr auto space = ToArray<1>(" ");

            constexpr auto semantic = ToArray<semantic_sv.size()>(semantic_sv);

            constexpr auto part1 = ConcatArrays(open_bracket, semantic);
            constexpr auto part2 = ConcatArrays(part1, close_bracket_colon);
            constexpr auto result = ConcatArrays(part2, space);

            return result;
        }

    public:
        [[nodiscard]] ConsoleOutputHandle CreateOutput(const std::string& filepath);

        template <typename _Severity, typename... _Args>
        requires IsConsoleSeverityLevelType<_Severity>
        static void Log(const std::format_string<_Args...>& message, _Args&&... args)
        {
            constexpr auto prefix = GetLogPrefixStyled<_Severity>();

            std::cout << prefix.data() << " " << std::format(message, std::forward<_Args>(args)...) << '\n';
        }

        template <typename _Severity, typename... _Args>
        requires IsConsoleSeverityLevelType<_Severity>
        static void Log(ConsoleOutputHandle outputHandle, const std::format_string<_Args...>& message, _Args&&... args);

        template <typename... _Args>
        [[noreturn]] static void Halt(const std::format_string<_Args...>& message, _Args&&... args);

        template <typename... _Args>
        [[noreturn]] void Halt(ConsoleOutputHandle outputHandle, const std::format_string<_Args...>& message, _Args&&... args);

        template <typename... _Args>
        [[noreturn]] static void Halt(std::int32_t exitCode, const std::format_string<_Args...>& message, _Args&&... args);

        template <typename... _Args>
        [[noreturn]] void Halt(std::int32_t exitCode, ConsoleOutputHandle outputHandle, const std::format_string<_Args...>& message, _Args&&... args);

    private:
        Console() = default;

        std::unordered_map<ConsoleOutputHandle, ConsoleFileOutput> mFileOutputs;
        std::unordered_map<std::string, ConsoleOutputHandle> mFilepathHandles;

        ConsoleOutputHandle mNextAvailableOutputHandle;

        template <typename T> requires std::is_base_of_v<Application, T>
        friend class Instance;
    };
}