#pragma once

#include <Mosaic/ECS/ECSManager.hpp>
#include <Mosaic/ECS/Entity.hpp>

#include <vector>

namespace Mosaic
{
    class ECSManager;

    template <typename... Components>
    class ECSView
    {
    public:
        struct Iterator
        {
            Iterator(ECSManager& manager, const std::vector<Entity>& entities, std::uint32_t index);

            bool operator!=(const Iterator& other) const;

            void operator++();

            std::tuple<Entity, Components&...> operator*() const;

        private:
            ECSManager& mManager;
            const std::vector<Entity>& mEntities;
            std::uint32_t mIndex;
        };

        Iterator begin();
        Iterator end();

    private:
        ECSView(ECSManager& manager);

        ECSManager& mManager;
        std::vector<Entity> mEntities;

        friend class ECSManager;
    };
}

#include <Mosaic/Inline/ECS/ECSView.inl>