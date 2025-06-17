#include <mosaic/ecs/entity.hpp>

namespace Mosaic::ECS
{
    bool Entity::operator==(const Entity& other) const
    {
        return ID == other.ID and Generation == other.Generation;
    }
}