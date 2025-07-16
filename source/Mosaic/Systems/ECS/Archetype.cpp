#include <Mosaic/Systems/ECS/Archetype.hpp>

namespace Mosaic::Systems::ECS
{
    bool Archetype::Insert(std::uint32_t id, EntityHandle entity)
    {
        return mEntities.Insert(id, entity);
    }

    bool Archetype::Remove(std::uint32_t id)
    {
        return mEntities.Remove(id);
    }

    SparseSet<EntityHandle>& Archetype::GetEntities()
    {
        return mEntities;
    }

    std::size_t Archetype::Size() const
    {
        return mEntities.Size();
    }
}