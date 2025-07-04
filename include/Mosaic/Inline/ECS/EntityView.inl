#pragma once

#include <Mosaic/ECS/EntityView.hpp>

#include <tuple>

namespace Mosaic
{
    template <typename... Components>
    EntityView<Components...>::Iterator::Iterator(EntityManager& manager, const std::vector<Entity>& entities, std::uint32_t index)
        : mManager(manager), mEntities(entities), mIndex(index)
    {
    }

    template <typename... Components>
    bool EntityView<Components...>::Iterator::operator!=(const Iterator& other) const
    {
        return mIndex != other.mIndex;
    }

    template <typename... Components>
    void EntityView<Components...>::Iterator::operator++()
    {
        mIndex++;
    }

    template <typename... Components>
    std::tuple<Entity, Components&...> EntityView<Components...>::Iterator::operator*() const
    {
        Entity entity = mEntities[mIndex];

        return std::tuple<Entity, Components&...>(entity, *mManager.GetComponentSet<Components>()->Get(entity)...);
    }

    template <typename... Components>
    EntityView<Components...>::Iterator EntityView<Components...>::begin()
    {
        return Iterator(mManager, mEntities, 0);
    }

    template <typename... Components>
    EntityView<Components...>::Iterator EntityView<Components...>::end()
    {
        return Iterator(mManager, mEntities, mEntities.size());
    }

    template <typename... Components>
    EntityView<Components...>::EntityView(EntityManager& manager)
        : mManager(manager)
    {
        auto* primarySet = mManager.GetComponentSet<std::tuple_element_t<0, std::tuple<Components...>>>();

        if (primarySet == nullptr)
        {
            return;
        }

        for (const Entity& entity : primarySet->Entities)
        {
            if ((mManager.GetComponentSet<Components>()->Has(entity) && ...))
            {
                mEntities.push_back(entity);
            }
        }
    }
}