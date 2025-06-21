#include <Mosaic/Application/Application.hpp>

namespace Mosaic
{
    Application::Application(InstanceResources& resources)
        : mInstanceResources(&resources)
    {
    }

    InstanceResources& Application::GetInstanceResources()
    {
        return *mInstanceResources;
    }
}