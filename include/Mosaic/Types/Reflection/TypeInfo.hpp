#pragma once

#include <Mosaic/Types/Constraints/Support.hpp>

#include <cstddef>
#include <string_view>

namespace Mosaic::Types::Reflection
{
    template <typename T>
    requires Constraints::IsCompilerSupported
    class TypeInfo
    {
    private:
        static consteval std::string_view TypeName()
        {
#if defined(__clang__)
            constexpr std::string_view func = __PRETTY_FUNCTION__;
            constexpr std::string_view prefix = "T = ";
            constexpr std::string_view suffix = ";";
#elif defined(__GNUC__)
            constexpr std::string_view func = __PRETTY_FUNCTION__;
            constexpr std::string_view prefix = "T = ";
            constexpr std::string_view suffix = ";";
#elif defined(_MSC_VER)
            constexpr std::string_view func = __FUNCSIG__;
            constexpr std::string_view prefix = "type_name<";
            constexpr std::string_view suffix = ">(void)";
#else
            return "ERROR_UNSUPPORTED_COMPILER";
#endif

            const auto start = func.find(prefix) + prefix.size();
            const auto end = func.rfind(suffix);

            return func.substr(start, end - start);
        }

    public:
        TypeInfo() = delete;

        static constexpr std::string_view Name = TypeName();

        static constexpr std::size_t Size = sizeof(T);

        using Type = T;
    };
}