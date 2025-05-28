#include "../../include/frontend/instance.hpp"

namespace Mosaic::Frontend
{
    std::uint32_t Instance::Run()
    {
        try
        {
            Setup();

            GlobalContext.Start();
            GlobalContext.Update();
            GlobalContext.Stop();

            return 0;
        }
        catch (const std::exception& error)
        {
            LogError("Fatal runtime exception: {}", error.what());

            return 1;
        }
    }
}