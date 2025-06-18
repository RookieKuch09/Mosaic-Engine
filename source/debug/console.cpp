#include <mosaic/debug/console.hpp>

#include <mosaic/api/pid.hpp>

#include <chrono>
#include <iostream>
#include <sstream>

namespace Mosaic::Debug
{
    Console::Console()
        : mNextAvailableID(1)
    {
    }

    auto Console::CreateFileOutput(const std::string& filepath) -> Console::OutputID
    {
        OutputID outputID = 0;

        if (not mFilepathIDs.contains(filepath))
        {
            outputID = mNextAvailableID;

            mNextAvailableID++;

            std::ofstream stream(filepath, std::ios::app);

            if (not stream)
            {
                Throw("Failed to create or open log file {}", filepath);
            }

            AddInitialLogstamp(stream, filepath);

            mFileOutputs.emplace(outputID, std::move(stream));

            mFilepathIDs[filepath] = outputID;
        }
        else
        {
            outputID = mFilepathIDs[filepath];
        }

        return outputID;
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

    auto Console::GetTimestamp() -> std::string
    {
        auto nowTimePoint = std::chrono::system_clock::now();
        auto nowTimeT = std::chrono::system_clock::to_time_t(nowTimePoint);
        auto localTime = *std::localtime(&nowTimeT);

        std::ostringstream buffer;

        buffer << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");

        return buffer.str();
    }

    void Console::AddInitialLogstamp(std::ofstream& file, const std::string& filepath)
    {
        auto pid = MOSAIC_GET_PID();
        auto timestamp = GetTimestamp();

        std::ifstream checkFile(filepath, std::ios::ate | std::ios::binary);

        const bool isNonEmpty = checkFile.tellg() > 0;

        auto sessionHeader = std::format(
            "{}--- Mosaic Log Session Started: {} | PID: {} | Filepath: {} ---\n",
            isNonEmpty ? "\n" : "",
            timestamp,
            pid,
            filepath);

        file << sessionHeader;
        file.flush();
    }
}