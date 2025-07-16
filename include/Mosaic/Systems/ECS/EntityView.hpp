#pragma once

#include <Mosaic/Systems/ECS/Entity.hpp>
#include <Mosaic/Systems/ECS/SparseSet.hpp>

namespace Mosaic::Systems::ECS
{
    template <typename Registry, typename... Components>
    class EntityViewIterator
    {
    public:
        EntityViewIterator(Registry* registry, SparseSet<EntityHandle>* entities, std::size_t index, std::size_t end);

        auto operator*() const;
        bool operator!=(const EntityViewIterator&) const;

        EntityViewIterator& operator++();

    public:
        Registry* mRegistry;
        SparseSet<EntityHandle>* mEntities;

        std::size_t mIndex;
        std::size_t mEnd;
    };

    template <typename Registry, typename... Components>
    class EntityView
    {
    public:
        EntityView(Registry* registry, SparseSet<EntityHandle>& entities);

        auto begin();
        auto end();

    private:
        Registry* mRegistry;
        SparseSet<EntityHandle>* mEntities;
    };
}

#include <Mosaic/Inline/Systems/ECS/EntityView.inl>
