#include <Mosaic/ECS/Archetype.hpp>

namespace Mosaic::ECS
{
    bool Archetype::Insert(std::uint32_t id, Entity entity)
    {
        return mEntities.Insert(id, entity);
    }

    bool Archetype::Remove(std::uint32_t id)
    {
        return mEntities.Remove(id);
    }

    Containers::SparseSet<Entity>& Archetype::GetEntities()
    {
        return mEntities;
    }

    std::size_t Archetype::Size() const
    {
        return mEntities.DenseSize();
    }
}