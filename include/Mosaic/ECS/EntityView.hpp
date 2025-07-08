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
            Iterator(EntityManager& manager, const std::vector<EntityHandle>& entities, std::uint32_t index);

            bool operator!=(const Iterator& other) const;

            void operator++();

            std::tuple<EntityHandle, Components&...> operator*() const;

        private:
            EntityManager& mManager;
            const std::vector<EntityHandle>& mEntities;
            std::uint32_t mIndex;
        };

        Iterator begin();
        Iterator end();

    private:
        EntityView(EntityManager& manager);

        EntityManager& mManager;
        std::vector<EntityHandle> mEntities;

        friend class EntityManager;
    };
}

#include <Mosaic/Inline/ECS/EntityView.inl>