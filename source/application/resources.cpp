#include <mosaic/application/resources.hpp>

namespace Mosaic
{
    ApplicationResources::ApplicationResources(class Console& console, class Window& window, class ECSManager& ecsManager, class EventManager& eventManager)
        : Console(console), Window(window), ECSManager(ecsManager), EventManager(eventManager)
    {
    }
}