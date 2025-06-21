#pragma once

#include <Mosaic/ECS/ECSView.hpp>

#include <tuple>

namespace Mosaic
{
    template <typename... Components>
    ECSView<Components...>::Iterator::Iterator(ECSManager& manager, const std::vector<Entity>& entities, std::uint32_t index)
        : mManager(manager), mEntities(entities), mIndex(index)
    {
    }

    template <typename... Components>
    bool ECSView<Components...>::Iterator::operator!=(const Iterator& other) const
    {
        return mIndex != other.mIndex;
    }

    template <typename... Components>
    void ECSView<Components...>::Iterator::operator++()
    {
        mIndex++;
    }

    template <typename... Components>
    std::tuple<Entity, Components&...> ECSView<Components...>::Iterator::operator*() const
    {
        Entity entity = mEntities[mIndex];

        return std::tuple<Entity, Components&...>(entity, *mManager.GetComponentSet<Components>()->Get(entity)...);
    }

    template <typename... Components>
    ECSView<Components...>::Iterator ECSView<Components...>::begin()
    {
        return Iterator(mManager, mEntities, 0);
    }

    template <typename... Components>
    ECSView<Components...>::Iterator ECSView<Components...>::end()
    {
        return Iterator(mManager, mEntities, mEntities.size());
    }

    template <typename... Components>
    ECSView<Components...>::ECSView(ECSManager& manager)
        : mManager(manager)
    {
        auto* primarySet = mManager.GetComponentSet<std::tuple_element_t<0, std::tuple<Components...>>>();

        if (primarySet == nullptr)
        {
            return;
        }

        for (const Entity& entity : primarySet->Entities)
        {
            if ((mManager.GetComponentSet<Components>()->Has(entity) and ...))
            {
                mEntities.push_back(entity);
            }
        }
    }
}