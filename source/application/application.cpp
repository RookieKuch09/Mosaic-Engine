#include <mosaic/application/application.hpp>

namespace Mosaic
{
    Application::Application(ApplicationResources& resources)
        : mApplicationResources(&resources)
    {
    }

    ApplicationResources& Application::GetApplicationResources()
    {
        return *mApplicationResources;
    }
}