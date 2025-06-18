#pragma once

#include <mosaic/debug/console.hpp>

#include <mosaic/api/exposure.hpp>

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
        auto operator=(const SparseSetInterface& other) -> SparseSetInterface& = default;

        SparseSetInterface(SparseSetInterface&& other) noexcept = default;
        auto operator=(SparseSetInterface&& other) noexcept -> SparseSetInterface& = default;

        virtual ~SparseSetInterface() = default;
    };

    template <typename Component>
    struct MOSAIC_PUBLIC_EXPOSURE SparseSet : SparseSetInterface
    {
        SparseSet() = default;

        SparseSet(const SparseSet& other) = default;
        auto operator=(const SparseSet& other) -> SparseSet& = default;

        SparseSet(SparseSet&& other) noexcept = default;
        auto operator=(SparseSet&& other) noexcept -> SparseSet& = default;

        ~SparseSet() override = default;

        void Insert(Console& console, const Entity& entity, const Component& component);
        void Remove(Console& console, const Entity& entity);

        [[nodiscard]] auto Get(const Entity& entity) -> Component*;
        [[nodiscard]] auto Has(const Entity& entity) const -> bool;

        std::vector<Component> Components;
        std::vector<Entity> Entities;

        std::unordered_map<EntityID, std::uint32_t> EntityIndex;
    };
}

#include <mosaic/inline/ecs/set.inl>