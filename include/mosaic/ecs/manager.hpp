#pragma once

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

    template <typename T> requires std::is_base_of<Application, T>::value
    class Instance;

    namespace ECS
    {
        using System = std::function<void(Resources&)>;

        template <typename... Components>
        struct MOSAIC_PUBLIC_EXPOSURE EntityBlueprint
        {
            using ComponentsTuple = std::tuple<Components...>;
            static constexpr std::size_t ComponentCount = sizeof...(Components);

            template <std::size_t I>
            using ComponentType = std::tuple_element_t<I, ComponentsTuple>;
        };

        class MOSAIC_PUBLIC_EXPOSURE Manager
        {
        public:
            Manager(const Manager& other) = delete;
            Manager& operator=(const Manager& other) = delete;

            Entity CreateEntity();

            template <typename Blueprint>
            Entity CreateEntity();

            std::vector<Entity> CreateEntities(size_t count);

            template <typename Blueprint>
            std::vector<Entity> CreateEntities(size_t count);

            template <typename Component>
            void AddComponent(Entity entity, const Component& component = Component{});

            template <typename Blueprint>
            void AddComponents(Entity entity);

            template <typename Component>
            void RemoveComponent(Entity entity);

            void DestroyEntity(Entity entity);

            void DestroyEntities(const std::vector<Entity>& entities);

            bool IsAlive(Entity entity) const;

            template <typename... Components>
            void ForEach(auto&& what);

            void AddSystem(const System& system);

        private:
            Manager(Resources& resources);

            void Update();

            template <typename Component>
            SparseSet<Component>* GetComponentSet();

            template <typename Blueprint, std::size_t... Is>
            void AddComponentsImpl(Entity entity, std::index_sequence<Is...>);

            std::unordered_map<std::type_index, std::unique_ptr<SparseSetInterface>> mComponentStorage;

            std::vector<System> mSystems;
            std::vector<EntityGeneration> mGenerations;
            std::vector<EntityID> mFreedIDs;

            Resources& mResources;

            template <typename T> requires std::is_base_of<Application, T>::value
            friend class Mosaic::Instance;

            friend struct Mosaic::Resources;
        };
    };
}

#include <mosaic/inline/ecs/manager.inl>