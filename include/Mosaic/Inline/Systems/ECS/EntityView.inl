#pragma once

#include <Mosaic/Systems/ECS/EntityView.hpp>

namespace Mosaic::Systems::ECS
{
    template <typename Registry, typename... Components>
    EntityViewIterator<Registry, Components...>::EntityViewIterator(Registry* registry, SparseSet<EntityHandle>* entities, std::size_t index, std::size_t end)
        : mRegistry(registry), mEntities(entities), mIndex(index), mEnd(end)
    {
    }

    template <typename Registry, typename... Components>
    auto EntityViewIterator<Registry, Components...>::operator*() const
    {
        EntityHandle entity = mEntities->TrustedGetFromDense(mIndex);

        return std::tuple<EntityHandle, Components&...>(entity, mRegistry->template TrustedGetComponent<Components>(entity)...);
    }

    template <typename Registry, typename... Components>
    bool EntityViewIterator<Registry, Components...>::operator!=(const EntityViewIterator<Registry, Components...>&) const
    {
        return mIndex < mEnd;
    }

    template <typename Registry, typename... Components>
    EntityViewIterator<Registry, Components...>& EntityViewIterator<Registry, Components...>::operator++()
    {
        ++mIndex;

        return *this;
    }

    template <typename Registry, typename... Components>
    EntityView<Registry, Components...>::EntityView(Registry* registry, SparseSet<EntityHandle>& entities)
        : mRegistry(registry), mEntities(&entities)
    {
    }

    template <typename Registry, typename... Components>
    auto EntityView<Registry, Components...>::begin()
    {
        std::size_t end = mEntities->Size();

        return EntityViewIterator<Registry, Components...>(mRegistry, mEntities, 0, end);
    }

    template <typename Registry, typename... Components>
    auto EntityView<Registry, Components...>::end()
    {
        std::size_t end = mEntities->Size();

        return EntityViewIterator<Registry, Components...>(mRegistry, mEntities, end, end);
    }
}