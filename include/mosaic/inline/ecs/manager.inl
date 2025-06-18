#pragma once

#include <mosaic/debug/console.hpp>

#include <mosaic/ecs/entity.hpp>
#include <mosaic/ecs/manager.hpp>

#include <boost/type_index.hpp>

namespace Mosaic::ECS
{
    template <typename Blueprint>
    auto Manager::CreateEntity() -> Entity
    {
        Entity entity = CreateEntity();

        AddComponents<Blueprint>(entity);

        return entity;
    }

    template <typename Blueprint>
    auto Manager::CreateEntities(uint32_t count) -> std::vector<Entity>
    {
        std::vector<Entity> entities;
        entities.reserve(count);

        for (std::uint32_t i = 0; i < count; i++)
        {
            entities.push_back(CreateEntity<Blueprint>());
        }

        return entities;
    }

    template <typename Component>
    void Manager::AddComponent(Entity entity, const Component& component)
    {
        auto typeIndex = std::type_index(typeid(Component));

        SparseSet<Component>* set = nullptr;

        if (not mComponentStorage.contains(typeIndex))
        {
            auto newSet = std::make_unique<SparseSet<Component>>();
            set = newSet.get();
            mComponentStorage[typeIndex] = std::move(newSet);
        }
        else
        {
            set = static_cast<SparseSet<Component>*>(mComponentStorage[typeIndex].get());
        }

        if (not set->Has(entity))
        {
            set->Insert(*mConsole, entity, component);
        }
        else
        {
            auto name = boost::typeindex::type_id<Component>().pretty_name();

            mConsole->Log<Debug::Console::LogSeverity::Warning>("Component of type {} already assigned to Entity {}", name, entity.ID);
        }
    }

    template <typename Blueprint>
    void Manager::AddComponents(Entity entity)
    {
        AddComponentsImpl<Blueprint>(entity, std::make_index_sequence<Blueprint::ComponentCount>{});
    }

    template <typename Component>
    void Manager::RemoveComponent(Entity entity)
    {
        auto typeIndex = std::type_index(typeid(Component));

        if (not mComponentStorage.contains(typeIndex))
        {
            return;
        }

        auto* set = static_cast<SparseSet<Component>*>(mComponentStorage[typeIndex].get());

        if (set->Has(entity))
        {
            set->Remove(*mConsole, entity);
        }
        else
        {
            auto name = boost::typeindex::type_id<Component>().pretty_name();

            mConsole->Log<Debug::Console::LogSeverity::Warning>("Component of type {} is not associated with Entity {}", name, entity.ID);
        }
    }

    template <typename... Components>
    auto Manager::QueryView() -> View<Components...>
    {
        return View<Components...>(this);
    }

    template <typename Component>
    auto Manager::GetComponentSet() -> SparseSet<Component>*
    {
        auto typeIndex = std::type_index(typeid(Component));

        if (not mComponentStorage.contains(typeIndex))
        {
            return nullptr;
        }

        return static_cast<SparseSet<Component>*>(mComponentStorage[typeIndex].get());
    }

    template <typename Blueprint, std::uint32_t... Is>
    void Manager::AddComponentsImpl(Entity entity)
    {
        (AddComponent<typename Blueprint::template ComponentType<Is>>(entity, {}), ...);
    }
}