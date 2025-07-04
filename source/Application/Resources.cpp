#include <Mosaic/Application/Instance.hpp>

namespace Mosaic
{
    InstanceResources::InstanceResources(class Console& console, class Window& window, class Renderer& renderer, class EntityManager& ecsManager, class EventManager& eventManager)
        : Console(console), Window(window), Renderer(renderer), EntityManager(ecsManager), EventManager(eventManager)
    {
    }
}