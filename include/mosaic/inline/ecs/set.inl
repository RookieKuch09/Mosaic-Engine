#pragma once

#include <mosaic/ecs/set.hpp>

namespace Mosaic::ECS
{
    template <typename Component>
    void SparseSet<Component>::Insert(Debug::Console& console, const Entity& entity, const Component& component)
    {
        if (EntityIndex.contains(entity.ID))
        {
            console.Log<Debug::Console::LogSeverity::Warning>("Entity {} already exists and cannot be recreated", entity.ID);

            return;
        }

        EntityIndex[entity.ID] = Components.size();
        Entities.emplace_back(entity);
        Components.emplace_back(component);
    }

    template <typename Component>
    void SparseSet<Component>::Remove(Debug::Console& console, const Entity& entity)
    {
        if (not EntityIndex.contains(entity.ID))
        {
            console.Log<Debug::Console::LogSeverity::Warning>("Entity {} does not exist and cannot be deleted", entity.ID);

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
    auto SparseSet<Component>::Get(const Entity& entity) -> Component*
    {
        if (not EntityIndex.contains(entity.ID))
        {
            return nullptr;
        }

        return &Components[EntityIndex[entity.ID]];
    }

    template <typename Component>
    auto SparseSet<Component>::Has(const Entity& entity) const -> bool
    {
        return EntityIndex.contains(entity.ID);
    }
}