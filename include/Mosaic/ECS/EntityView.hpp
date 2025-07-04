#pragma once

#include <Mosaic/ECS/Entity.hpp>
#include <Mosaic/ECS/EntityManager.hpp>

#include <vector>

namespace Mosaic
{
    class EntityManager;

    template <typename... Components>
    class EntityView
    {
    public:
        struct Iterator
        {
            Iterator(EntityManager& manager, const std::vector<Entity>& entities, std::uint32_t index);

            bool operator!=(const Iterator& other) const;

            void operator++();

            std::tuple<Entity, Components&...> operator*() const;

        private:
            EntityManager& mManager;
            const std::vector<Entity>& mEntities;
            std::uint32_t mIndex;
        };

        Iterator begin();
        Iterator end();

    private:
        EntityView(EntityManager& manager);

        EntityManager& mManager;
        std::vector<Entity> mEntities;

        friend class EntityManager;
    };
}

#include <Mosaic/Inline/ECS/EntityView.inl>