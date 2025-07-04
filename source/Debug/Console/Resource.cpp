#include <Mosaic/Debug/Console/Resource.hpp>

namespace Mosaic
{
    ConsoleOutputHandle Console::CreateOutput(const std::string& filepath)
    {
        auto it = mFilepathHandles.find(filepath);

        if (it != mFilepathHandles.end())
        {
            return it->second;
        }

        auto outputHandle = ConsoleOutputHandle(mNextAvailableOutputHandle);

        mNextAvailableOutputHandle++;

        auto& output = mFileOutputs[outputHandle];

        output.open(filepath, std::ios::app);

        if (!output)
        {
            Halt("Failed to create ConsoleOutputHandle with path {}", filepath);
        }

        return outputHandle;
    }

    Console::Console()
        : mNextAvailableOutputHandle(0)
    {
    }
}