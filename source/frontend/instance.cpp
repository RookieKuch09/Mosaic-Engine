#include <frontend/instance.hpp>

namespace Mosaic::Frontend
{
    std::uint32_t Instance::Run()
    {
        try
        {
            mGlobalContext.Start();
            mGlobalContext.Update();
            mGlobalContext.Close();

            return 0;
        }
        catch (const std::exception& error)
        {
            LogError("Fatal runtime exception: {}", error.what());

            return 1;
        }
    }
}