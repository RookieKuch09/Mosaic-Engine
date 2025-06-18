#pragma once

#include "mosaic/debug/console.hpp"
#include <mosaic/api/exposure.hpp>

#include <mosaic/ecs/entity.hpp>
#include <mosaic/ecs/set.hpp>

#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>

namespace Mosaic
{
    struct Resources;
    class Application;

    template <typename T> requires std::is_base_of_v<Application, T>
    class Instance;

    namespace Debug
    {
        class Console;
    }

    namespace ECS
    {
        using System = std::function<void(Resources&)>;

        template <typename... Components>
        struct MOSAIC_PUBLIC_EXPOSURE EntityBlueprint
        {
            using ComponentsTuple = std::tuple<Components...>;
            static constexpr std::uint32_t ComponentCount = sizeof...(Components);

            template <std::uint32_t I>
            using ComponentType = std::tuple_element_t<I, ComponentsTuple>;
        };

        class MOSAIC_PUBLIC_EXPOSURE Manager
        {
        public:
            Manager(const Manager& other) = delete;
            Manager(Manager&& other) noexcept = default;

            auto operator=(const Manager& other) -> Manager& = delete;
            auto operator=(Manager&& other) noexcept -> Manager& = default;

            ~Manager() = default;

            [[nodiscard]] auto CreateEntity() -> Entity;

            template <typename Blueprint>
            auto CreateEntity() -> Entity;

            [[nodiscard]] auto CreateEntities(std::uint32_t count) -> std::vector<Entity>;

            template <typename Blueprint>
            auto CreateEntities(std::uint32_t count) -> std::vector<Entity>;

            template <typename Component>
            void AddComponent(Entity entity, const Component& component = Component{});

            template <typename Blueprint>
            void AddComponents(Entity entity);

            template <typename Component>
            void RemoveComponent(Entity entity);

            void DestroyEntity(Entity entity);

            void DestroyEntities(const std::vector<Entity>& entities);

            [[nodiscard]] auto IsAlive(Entity entity) const -> bool;

            template <typename... Components>
            void ForEach(auto&& what);

            void AddSystem(const System& system);

        private:
            Manager(Resources& resources, Debug::Console& console);

            void Update();

            template <typename Component>
            auto GetComponentSet() -> SparseSet<Component>*;

            template <typename Blueprint, std::uint32_t... Is>
            void AddComponentsImpl(Entity entity);

            std::unordered_map<std::type_index, std::unique_ptr<SparseSetInterface>> mComponentStorage;

            std::vector<System> mSystems;
            std::vector<EntityGeneration> mGenerations;
            std::vector<EntityID> mFreedIDs;

            Resources* mResources;

            Debug::Console* mConsole;

            template <typename T>
            requires std::is_base_of_v<Application, T>
            friend class Mosaic::Instance;

            friend struct Mosaic::Resources;
        };
    };
}

#include <mosaic/inline/ecs/manager.inl>