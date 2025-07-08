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
        EntityManager(const EntityManager&) = delete;
        EntityManager(EntityManager&&) noexcept = delete;

        EntityManager& operator=(const EntityManager&) = delete;
        EntityManager& operator=(EntityManager&&) noexcept = delete;

        [[nodiscard]] EntityHandle CreateEntity();

        [[nodiscard]] std::vector<EntityHandle> CreateEntities(std::uint32_t count);

        template <typename Component>
        void AddComponent(EntityHandle entity, const Component& component = Component{});

        template <typename Component>
        void RemoveComponent(EntityHandle entity);

        void DestroyEntity(EntityHandle entity);

        void DestroyEntities(const std::vector<EntityHandle>& entities);

        [[nodiscard]] bool EntityExists(EntityHandle entity) const;

        template <typename... Components>
        [[nodiscard]] EntityView<Components...> QueryView();

        void AddSystem(const EntitySystem& system);

    private:
        EntityManager(InstanceResources& resources);
        ~EntityManager() = default;

        void Update();

        template <typename Component>
        SparseSet<Component>* GetComponentSet();

        std::unordered_map<std::type_index, std::unique_ptr<SparseSetInterface>> mComponentStorage;

        std::vector<EntitySystem> mSystems;

        std::vector<std::uint32_t> mGenerations;
        std::vector<std::uint32_t> mFreedHandles;

        InstanceResources& mInstanceResources;

        template <typename T> requires std::is_base_of_v<Application, T>
        friend class Instance;

        template <typename... Components>
        friend class EntityView;
    };

}

#include <Mosaic/Inline/ECS/EntityManager.inl>