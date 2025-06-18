#pragma once

#include <mosaic/ecs/entity.hpp>
#include <mosaic/ecs/manager.hpp>

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

            auto operator!=(const Iterator& other) const -> bool;

            void operator++();

            auto operator*() const;

        private:
            ECSManager& mManager;
            const std::vector<Entity>& mEntities;
            std::uint32_t mIndex;
        };

        auto begin() -> Iterator;
        auto end() -> Iterator;

    private:
        ECSView(ECSManager& manager);

        ECSManager& mManager;
        std::vector<Entity> mEntities;

        friend class ECSManager;
    };
}

#include <mosaic/inline/ecs/view.inl>