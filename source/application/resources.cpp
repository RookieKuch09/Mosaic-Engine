#include <mosaic/application/resources.hpp>

namespace Mosaic
{
    Resources::Resources(class Console& console, class ECSManager& ecsManager, class EventManager& eventManager)
        : Console(console), ECSManager(ecsManager), EventManager(eventManager)
    {
    }
}