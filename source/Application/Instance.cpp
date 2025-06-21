#include <Mosaic/Application/Instance.hpp>

namespace Mosaic
{
    InstanceResources::InstanceResources(class Console& console, class Window& window, class ECSManager& ecsManager, class EventManager& eventManager)
        : Console(console), Window(window), ECSManager(ecsManager), EventManager(eventManager)
    {
    }
}