#include <Mosaic/Debug/Console/Resource.hpp>

namespace Mosaic
{
    ConsoleOutputHandle Console::CreateOutput(const std::string& filepath)
    {
        if (mFilepathHandles.contains(filepath))
        {
            return mFilepathHandles[filepath];
        }
        else
        {
            auto outputHandle = mNextAvailableOutputHandle;

            mNextAvailableOutputHandle++;

            auto& output = mFileOutputs[outputHandle];

            output.mFilepath = filepath;
            output.mStream.open(filepath, std::ios::app);

            if (!output.mStream)
            {
                Halt("Failed to create ConsoleOutputHandle with path {}", filepath);
            }

            return outputHandle;
        }
    }

    Console::Console()
        : mNextAvailableOutputHandle(0)
    {
    }
}