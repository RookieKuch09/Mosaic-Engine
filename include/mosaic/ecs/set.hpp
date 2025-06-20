#pragma once

#include <mosaic/debug/console.hpp>

#include <mosaic/macros/exposure.hpp>

#include <mosaic/ecs/entity.hpp>

#include <cassert>
#include <unordered_map>
#include <vector>

namespace Mosaic
{
    struct MOSAIC_PUBLIC_EXPOSURE SparseSetInterface
    {
        SparseSetInterface() = default;

        SparseSetInterface(const SparseSetInterface& other) = default;
        SparseSetInterface& operator=(const SparseSetInterface& other) = default;

        SparseSetInterface(SparseSetInterface&& other) noexcept = default;
        SparseSetInterface& operator=(SparseSetInterface&& other) noexcept = default;

        virtual ~SparseSetInterface() = default;
    };

    template <typename Component>
    struct MOSAIC_PUBLIC_EXPOSURE SparseSet : SparseSetInterface
    {
        SparseSet() = default;

        SparseSet(const SparseSet& other) = default;
        SparseSet& operator=(const SparseSet& other) = default;

        SparseSet(SparseSet&& other) noexcept = default;
        SparseSet& operator=(SparseSet&& other) noexcept = default;

        ~SparseSet() override = default;

        void Insert(Console& console, const Entity& entity, const Component& component);
        void Remove(Console& console, const Entity& entity);

        [[nodiscard]] Component* Get(const Entity& entity);
        [[nodiscard]] bool Has(const Entity& entity) const;

        std::vector<Component> Components;
        std::vector<Entity> Entities;

        std::unordered_map<EntityID, std::uint32_t> EntityIndex;
    };
}

#include <mosaic/inline/ecs/set.inl>