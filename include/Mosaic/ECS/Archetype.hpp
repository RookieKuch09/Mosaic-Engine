#pragma once

#include <Mosaic/Macros/Symbols.hpp>

#include <Mosaic/Containers/SparseSet.hpp>

#include <Mosaic/ECS/Entity.hpp>

namespace Mosaic::ECS
{
    class MOSAIC_PUBLIC_SYMBOL Archetype
    {
    public:
        Archetype() = default;
        ~Archetype() = default;

        Archetype(const Archetype&) = delete;
        Archetype(Archetype&&) noexcept = default;

        Archetype& operator=(const Archetype&) = delete;
        Archetype& operator=(Archetype&&) noexcept = default;

        [[nodiscard]] bool Insert(std::uint32_t id, Entity entity);
        [[nodiscard]] bool Remove(std::uint32_t id);

        Containers::SparseSet<Entity>& GetEntities();

        std::size_t Size() const;

    private:
        Containers::SparseSet<Entity> mEntities;
    };
}