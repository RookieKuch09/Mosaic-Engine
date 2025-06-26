#include <Mosaic/ECS/Entity.hpp>

namespace Mosaic
{
    bool Entity::operator==(const Entity& other) const
    {
        return ID == other.ID && Generation == other.Generation;
    }
}