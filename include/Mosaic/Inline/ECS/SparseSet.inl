#pragma once

#include <Mosaic/ECS/SparseSet.hpp>

namespace Mosaic
{
    template <typename Component>
    void SparseSet<Component>::Insert(Console&, const EntityHandle& entity, const Component& component)
    {
        if (EntityIndex.contains(entity))
        {
            // TODO: replace with log statement

            return;
        }

        EntityIndex[entity] = Components.size();
        Entities.emplace_back(entity);
        Components.emplace_back(component);
    }

    template <typename Component>
    void SparseSet<Component>::Remove(Console&, const EntityHandle& entity)
    {
        if (!EntityIndex.contains(entity))
        {
            // TODO: replace with log statement

            return;
        }

        std::uint32_t index = EntityIndex[entity];
        EntityHandle lastEntity = Entities.back();

        Components[index] = Components.back();
        Entities[index] = lastEntity;
        EntityIndex[lastEntity] = index;

        Components.pop_back();
        Entities.pop_back();
        EntityIndex.erase(entity);
    }

    template <typename Component>
    Component* SparseSet<Component>::Get(const EntityHandle& entity)
    {
        if (!EntityIndex.contains(entity))
        {
            return nullptr;
        }

        return &Components[EntityIndex[entity]];
    }

    template <typename Component>
    bool SparseSet<Component>::Has(const EntityHandle& entity) const
    {
        return EntityIndex.contains(entity);
    }
}