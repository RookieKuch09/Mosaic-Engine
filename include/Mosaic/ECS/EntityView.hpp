#pragma once

#include <Mosaic/Containers/SparseSet.hpp>

#include <Mosaic/ECS/Entity.hpp>

namespace Mosaic::ECS
{
    template <typename ECS, typename... Components>
    class EntityViewIterator
    {
    public:
        EntityViewIterator(ECS* registry, Containers::SparseSet<Entity>* entities, std::size_t index, std::size_t end);

        auto operator*() const;
        bool operator!=(const EntityViewIterator&) const;

        EntityViewIterator& operator++();

    public:
        ECS* mRegistry;

        Containers::SparseSet<Entity>* mEntities;

        std::size_t mIndex;
        std::size_t mEnd;
    };

    template <typename ECS, typename... Components>
    class EntityView
    {
    public:
        EntityView(ECS* registry, Containers::SparseSet<Entity>& entities);

        auto begin();
        auto end();

        auto begin() const;
        auto end() const;

    private:
        ECS* mRegistry;

        Containers::SparseSet<Entity>* mEntities;
    };
}

#include <Mosaic/Inline/ECS/EntityView.inl>
