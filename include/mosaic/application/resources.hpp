#pragma once

#include <mosaic/api/exposure.hpp>

namespace Mosaic
{
    class Console;
    class ECSManager;
    class EventManager;

    struct MOSAIC_PUBLIC_EXPOSURE Resources
    {
        Resources(Console& console, ECSManager& ecsManager, EventManager& eventManager);

        Console& Console;
        ECSManager& ECSManager;
        EventManager& EventManager;
    };
}