#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace Mosaic
{
    class Resources;

    class ECS
    {
    public:
        using EntityID = std::uint32_t;
        using EntityGeneration = std::uint32_t;

        struct Entity
        {
            EntityID ID;
            EntityGeneration Generation;

            bool operator==(const Entity& other) const;
        };

        template <typename Component>
        struct SparseSet
        {
            std::vector<Component> Components;
            std::vector<Entity> Entities;

            std::unordered_map<EntityID, std::uint32_t> EntityIndex;
        };

        using System = std::function<void(Resources&)>;

        class Manager
        {
        public:
        private:
            Manager() = default;

            std::unordered_map<std::type_index, std::unique_ptr<void>> mComponentStorage;

            friend class Instance;
        };
    };
}