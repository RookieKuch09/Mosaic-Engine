#include "../../include/systems/instance.hpp"
#include "../../include/systems/contexts.hpp"

namespace Mosaic::Systems
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