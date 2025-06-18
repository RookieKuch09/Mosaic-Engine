#include <mosaic/application/application.hpp>

namespace Mosaic
{
    Application::Application(Resources& resources)
        : mApplicationResources(&resources)
    {
    }

    auto Application::GetApplicationResources() -> Resources&
    {
        return *mApplicationResources;
    }
}