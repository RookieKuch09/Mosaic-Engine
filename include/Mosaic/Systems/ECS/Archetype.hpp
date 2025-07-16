#pragma once

#include <Mosaic/Systems/ECS/Entity.hpp>
#include <Mosaic/Systems/ECS/SparseSet.hpp>

namespace Mosaic::Systems::ECS
{
    class Archetype
    {
    public:
        Archetype() = default;
        ~Archetype() = default;

        Archetype(const Archetype&) = delete;
        Archetype(Archetype&&) noexcept = default;

        Archetype& operator=(const Archetype&) = delete;
        Archetype& operator=(Archetype&&) noexcept = default;

        [[nodiscard]] bool Insert(std::uint32_t id, EntityHandle entity);
        [[nodiscard]] bool Remove(std::uint32_t id);

        SparseSet<EntityHandle>& GetEntities();

        std::size_t Size() const;

    private:
        SparseSet<EntityHandle> mEntities;
    };
}