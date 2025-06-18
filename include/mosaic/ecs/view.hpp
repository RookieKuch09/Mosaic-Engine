#pragma once

#include <mosaic/ecs/entity.hpp>
#include <mosaic/ecs/manager.hpp>

#include <vector>

namespace Mosaic::ECS
{
    class Manager;

    template <typename... Components>
    class View
    {
    public:
        struct Iterator
        {
            Iterator(Manager* manager, const std::vector<Entity>& entities, std::uint32_t index);

            auto operator!=(const Iterator& other) const -> bool;

            void operator++();

            auto operator*() const;

        private:
            Manager* mManager;
            const std::vector<Entity>* mEntities;
            std::uint32_t mIndex;
        };

        auto begin() -> Iterator;
        auto end() -> Iterator;

    private:
        View(Manager* manager);

        Manager* mManager;
        std::vector<Entity> mEntities;

        friend class Manager;
    };
}

#include <mosaic/inline/ecs/view.inl>