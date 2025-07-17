#pragma once

#include <Mosaic/ECS/EntityView.hpp>

namespace Mosaic::ECS
{
    template <typename ECS, typename... Components>
    EntityViewIterator<ECS, Components...>::EntityViewIterator(ECS* registry, Containers::SparseSet<Entity>* entities, std::size_t index, std::size_t end)
        : mRegistry(registry), mEntities(entities), mIndex(index), mEnd(end)
    {
    }

    template <typename ECS, typename... Components>
    auto EntityViewIterator<ECS, Components...>::operator*() const
    {
        Entity entity = mEntities->UnsafeGetFromDense(mIndex);

        return std::tuple<Entity, Components&...>(entity, mRegistry->template UnsafeGetComponent<Components>(entity)...);
    }

    template <typename ECS, typename... Components>
    bool EntityViewIterator<ECS, Components...>::operator!=(const EntityViewIterator<ECS, Components...>&) const
    {
        return mIndex < mEnd;
    }

    template <typename ECS, typename... Components>
    EntityViewIterator<ECS, Components...>& EntityViewIterator<ECS, Components...>::operator++()
    {
        ++mIndex;

        return *this;
    }

    template <typename ECS, typename... Components>
    EntityView<ECS, Components...>::EntityView(ECS* registry, Containers::SparseSet<Entity>& entities)
        : mRegistry(registry), mEntities(&entities)
    {
    }

    template <typename ECS, typename... Components>
    auto EntityView<ECS, Components...>::begin()
    {
        std::size_t end = mEntities->DenseSize();

        return EntityViewIterator<ECS, Components...>(mRegistry, mEntities, 0, end);
    }

    template <typename ECS, typename... Components>
    auto EntityView<ECS, Components...>::end()
    {
        std::size_t end = mEntities->DenseSize();

        return EntityViewIterator<ECS, Components...>(mRegistry, mEntities, end, end);
    }

    template <typename ECS, typename... Components>
    auto EntityView<ECS, Components...>::begin() const
    {
        std::size_t end = mEntities->DenseSize();

        return EntityViewIterator<ECS, Components...>(mRegistry, mEntities, 0, end);
    }

    template <typename ECS, typename... Components>
    auto EntityView<ECS, Components...>::end() const
    {
        std::size_t end = mEntities->DenseSize();

        return EntityViewIterator<ECS, Components...>(mRegistry, mEntities, end, end);
    }
}