#include <mosaic/debug/console.hpp>

#include <chrono>
#include <functional>
#include <iostream>
#include <sstream>

namespace Mosaic::Debug
{
    Console::OutputID Console::CreateFileOutputID(const std::string& filepath)
    {
        OutputID id = HashFilePath(filepath);

        if (mFileOutputs.find(id) == mFileOutputs.end())
        {
            std::ofstream stream(filepath, std::ios::app);

            if (not stream)
            {
                // TODO: replace with engine API assertion
                throw std::runtime_error("Failed to open log file: " + filepath);
            }

            mFileOutputs.emplace(id, std::move(stream));
        }

        return id;
    }

    void Console::DispatchToTerminal(const std::string& message)
    {
        std::cout << message << '\n';
    }

    void Console::DispatchToFile(FileOutput& fileOutput, const std::string& message)
    {
        fileOutput.Stream << message << '\n';
        fileOutput.Stream.flush();
    }

    Console::OutputID Console::HashFilePath(const std::string& filepath)
    {
        return static_cast<OutputID>(std::hash<std::string>{}(filepath));
    }

    std::string Console::GetTimestamp()
    {
        auto nowTimePoint = std::chrono::system_clock::now();
        auto nowTimeT = std::chrono::system_clock::to_time_t(nowTimePoint);
        auto localTime = *std::localtime(&nowTimeT);

        std::ostringstream buffer;

        buffer << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");

        return buffer.str();
    }
}