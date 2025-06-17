#pragma once

#include <mosaic/api/exposure.hpp>

#include <mosaic/ecs/entity.hpp>

#include <cassert>
#include <unordered_map>
#include <vector>

namespace Mosaic::ECS
{
    struct SparseSetInterface
    {
        virtual ~SparseSetInterface() = default;
    };

    template <typename Component>
    struct MOSAIC_PUBLIC_EXPOSURE SparseSet : SparseSetInterface
    {
        std::vector<Component> Components;
        std::vector<Entity> Entities;

        std::unordered_map<EntityID, std::uint32_t> EntityIndex;

        void Insert(const Entity& entity, const Component& component)
        {
            if (EntityIndex.find(entity.ID) != EntityIndex.end())
            {
                // TODO: log warning about entity already existing
                return;
            }

            EntityIndex[entity.ID] = Components.size();
            Entities.emplace_back(entity);
            Components.emplace_back(component);
        }

        void Remove(const Entity& entity)
        {
            auto it = EntityIndex.find(entity.ID);

            if (it == EntityIndex.end())
            {
                return;
            }

            std::uint32_t index = it->second;
            Entity lastEntity = Entities.back();

            Components[index] = Components.back();
            Entities[index] = lastEntity;
            EntityIndex[lastEntity.ID] = index;

            Components.pop_back();
            Entities.pop_back();
            EntityIndex.erase(it);
        }

        Component* Get(const Entity& entity)
        {
            auto it = EntityIndex.find(entity.ID);

            if (it == EntityIndex.end())
            {
                return nullptr;
            }

            return &Components[it->second];
        }

        bool Has(const Entity& entity) const
        {
            return EntityIndex.find(entity.ID) != EntityIndex.end();
        }
    };
}