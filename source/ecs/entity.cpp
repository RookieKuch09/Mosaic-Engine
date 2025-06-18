#include <mosaic/ecs/entity.hpp>

namespace Mosaic::ECS
{
    auto Entity::operator==(const Entity& other) const -> bool
    {
        return ID == other.ID and Generation == other.Generation;
    }
}