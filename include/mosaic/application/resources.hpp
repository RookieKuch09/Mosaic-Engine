#pragma once

#include <mosaic/api/exposure.hpp>

#include <mosaic/debug/console.hpp>

#include <mosaic/ecs/manager.hpp>

namespace Mosaic
{
    struct Resources
    {
        Resources();

        Debug::Console Console;

        ECS::Manager ECSManager;
    };
}