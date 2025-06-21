#pragma once

#include <Mosaic/ECS/SparseSet.hpp>

namespace Mosaic
{
    template <typename Component>
    void SparseSet<Component>::Insert(Console& console, const Entity& entity, const Component& component)
    {
        if (EntityIndex.contains(entity.ID))
        {
            console.Log<Console::LogSeverity::Warning>("Entity {} already exists and cannot be recreated", entity.ID);

            return;
        }

        EntityIndex[entity.ID] = Components.size();
        Entities.emplace_back(entity);
        Components.emplace_back(component);
    }

    template <typename Component>
    void SparseSet<Component>::Remove(Console& console, const Entity& entity)
    {
        if (not EntityIndex.contains(entity.ID))
        {
            console.Log<Console::LogSeverity::Warning>("Entity {} does not exist and cannot be deleted", entity.ID);

            return;
        }

        std::uint32_t index = EntityIndex[entity.ID];
        Entity lastEntity = Entities.back();

        Components[index] = Components.back();
        Entities[index] = lastEntity;
        EntityIndex[lastEntity.ID] = index;

        Components.pop_back();
        Entities.pop_back();
        EntityIndex.erase(EntityIndex[entity.ID]);
    }

    template <typename Component>
    Component* SparseSet<Component>::Get(const Entity& entity)
    {
        if (not EntityIndex.contains(entity.ID))
        {
            return nullptr;
        }

        return &Components[EntityIndex[entity.ID]];
    }

    template <typename Component>
    bool SparseSet<Component>::Has(const Entity& entity) const
    {
        return EntityIndex.contains(entity.ID);
    }
}