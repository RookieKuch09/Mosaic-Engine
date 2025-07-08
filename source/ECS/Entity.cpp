#include <Mosaic/ECS/Entity.hpp>

namespace Mosaic
{
    bool EntityHandle::operator==(const EntityHandle& other) const
    {
        return ID == other.ID && Generation == other.Generation;
    }
}