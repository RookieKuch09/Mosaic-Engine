#include <Mosaic/ECS/Entity.hpp>

namespace Mosaic
{
    bool Entity::operator==(const Entity& other) const
    {
        return Handle == other.Handle && Generation == other.Generation;
    }
}