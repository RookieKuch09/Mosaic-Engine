#pragma once

#include <Mosaic/Macros/Exposure.hpp>

#include <Mosaic/ECS/SparseSet.hpp>

#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>

namespace Mosaic
{
    class InstanceResources;

    class Application;
    class Console;

    template <typename... Components>
    class EntityView;

    using EntitySystem = std::function<void(InstanceResources&)>;

    class MOSAIC_PUBLIC_EXPOSURE EntityManager
    {
    public:
        EntityManager(const EntityManager& other) = delete;
        EntityManager(EntityManager&& other) noexcept = default;

        auto operator=(const EntityManager& other) -> EntityManager& = delete;
        auto operator=(EntityManager&& other) noexcept -> EntityManager& = delete;

        ~EntityManager() = default;

        [[nodiscard]] Entity CreateEntity();

        [[nodiscard]] std::vector<Entity> CreateEntities(std::uint32_t count);

        template <typename Component>
        void AddComponent(Entity entity, const Component& component = Component{});

        template <typename Component>
        void RemoveComponent(Entity entity);

        void DestroyEntity(Entity entity);

        void DestroyEntities(const std::vector<Entity>& entities);

        [[nodiscard]] bool EntityExists(Entity entity) const;

        template <typename... Components>
        [[nodiscard]] EntityView<Components...> QueryView();

        void AddSystem(const EntitySystem& system);

    private:
        EntityManager(InstanceResources& resources);

        void Update();

        template <typename Component>
        SparseSet<Component>* GetComponentSet();

        std::unordered_map<std::type_index, std::unique_ptr<SparseSetInterface>> mComponentStorage;

        std::vector<EntitySystem> mSystems;
        std::vector<EntityGeneration> mGenerations;
        std::vector<EntityHandle> mFreedIDs;

        InstanceResources& mInstanceResources;

        template <typename T> requires std::is_base_of_v<Application, T>
        friend class Instance;

        template <typename... Components>
        friend class EntityView;
    };

}

#include <Mosaic/Inline/ECS/EntityManager.inl>