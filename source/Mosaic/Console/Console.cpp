#include <Mosaic/Console/Console.hpp>

#include <chrono>
#include <sstream>

namespace Mosaic::Console
{
    std::string Console::Timestamp() const
    {
        auto now = std::chrono::system_clock::now();

        auto nowTimeFormat = std::chrono::system_clock::to_time_t(now);
        auto localTime = *std::localtime(&nowTimeFormat);

        std::ostringstream buffer;

        buffer << TEXT_FOREGROUND_CYAN << TEXT_BOLD_INTENSITY << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");

        return buffer.str();
    }
}