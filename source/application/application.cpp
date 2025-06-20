#include <mosaic/application/application.hpp>

namespace Mosaic
{
    Application::Application(Resources& resources)
        : mApplicationResources(&resources)
    {
    }

    Resources& Application::GetApplicationResources()
    {
        return *mApplicationResources;
    }
}