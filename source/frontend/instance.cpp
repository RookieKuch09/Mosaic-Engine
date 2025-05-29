#include "../../include/frontend/instance.hpp"
#include "frontend/contexts.hpp"

namespace Mosaic::Frontend
{
    Instance::Instance(const std::string& configPath)
        : GlobalContext(configPath)
    {
    }

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
            Utilities::LogError("Fatal runtime exception: {}", error.what());

            return 1;
        }
    }
}