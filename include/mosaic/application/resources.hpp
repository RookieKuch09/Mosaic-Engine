#pragma once

#include <mosaic/macros/exposure.hpp>

namespace Mosaic
{
    class Console;
    class Window;

    class ECSManager;
    class EventManager;

    struct MOSAIC_PUBLIC_EXPOSURE ApplicationResources
    {
        ApplicationResources(Console& console, Window& window, ECSManager& ecsManager, EventManager& eventManager);

        Console& Console;
        Window& Window;

        ECSManager& ECSManager;
        EventManager& EventManager;
    };
}