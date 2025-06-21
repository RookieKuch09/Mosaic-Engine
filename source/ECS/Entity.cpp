#include <Mosaic/ECS/Entity.hpp>

namespace Mosaic
{
    bool Entity::operator==(const Entity& other) const
    {
        return ID == other.ID and Generation == other.Generation;
    }
}