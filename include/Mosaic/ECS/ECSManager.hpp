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
    class ECSView;

    using ECSSystem = std::function<void(InstanceResources&)>;

    class MOSAIC_PUBLIC_EXPOSURE ECSManager
    {
    public:
        ECSManager(const ECSManager& other) = delete;
        ECSManager(ECSManager&& other) noexcept = default;

        auto operator=(const ECSManager& other) -> ECSManager& = delete;
        auto operator=(ECSManager&& other) noexcept -> ECSManager& = delete;

        ~ECSManager() = default;

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
        [[nodiscard]] ECSView<Components...> QueryView();

        void AddSystem(const ECSSystem& system);

    private:
        ECSManager(InstanceResources& resources);

        void Update();

        template <typename Component>
        SparseSet<Component>* GetComponentSet();

        std::unordered_map<std::type_index, std::unique_ptr<SparseSetInterface>> mComponentStorage;

        std::vector<ECSSystem> mSystems;
        std::vector<EntityGeneration> mGenerations;
        std::vector<EntityID> mFreedIDs;

        InstanceResources& mInstanceResources;

        template <typename T> requires std::is_base_of_v<Application, T>
        friend class Instance;

        template <typename... Components>
        friend class ECSView;
    };

}

#include <Mosaic/Inline/ECS/ECSManager.inl>