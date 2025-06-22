#include <Mosaic/Application/Instance.hpp>

namespace Mosaic
{
    InstanceResources::InstanceResources(class Console& console, class Window& window, class Renderer& renderer, class ECSManager& ecsManager, class EventManager& eventManager)
        : Console(console), Window(window), Renderer(renderer), ECSManager(ecsManager), EventManager(eventManager)
    {
    }
}