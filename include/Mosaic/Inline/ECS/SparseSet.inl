#pragma once

#include <Mosaic/ECS/SparseSet.hpp>

namespace Mosaic
{
    template <typename Component>
    void SparseSet<Component>::Insert(Console&, const Entity& entity, const Component& component)
    {
        if (EntityIndex.contains(entity.Handle))
        {
            // TODO: replace with log statement

            return;
        }

        EntityIndex[entity.Handle] = Components.size();
        Entities.emplace_back(entity);
        Components.emplace_back(component);
    }

    template <typename Component>
    void SparseSet<Component>::Remove(Console&, const Entity& entity)
    {
        if (!EntityIndex.contains(entity.Handle))
        {
            // TODO: replace with log statement

            return;
        }

        std::uint32_t index = EntityIndex[entity.Handle];
        Entity lastEntity = Entities.back();

        Components[index] = Components.back();
        Entities[index] = lastEntity;
        EntityIndex[lastEntity.Handle] = index;

        Components.pop_back();
        Entities.pop_back();
        EntityIndex.erase(EntityIndex[entity.Handle]);
    }

    template <typename Component>
    Component* SparseSet<Component>::Get(const Entity& entity)
    {
        if (!EntityIndex.contains(entity.Handle))
        {
            return nullptr;
        }

        return &Components[EntityIndex[entity.Handle]];
    }

    template <typename Component>
    bool SparseSet<Component>::Has(const Entity& entity) const
    {
        return EntityIndex.contains(entity.Handle);
    }
}