#pragma once

#include <mosaic/ecs/manager.hpp>

namespace Mosaic::ECS
{
    template <typename Blueprint>
    Entity Manager::CreateEntity()
    {
        Entity entity = CreateEntity();

        AddComponents<Blueprint>(entity);

        return entity;
    }

    template <typename Blueprint>
    std::vector<Entity> Manager::CreateEntities(size_t count)
    {
        std::vector<Entity> entities;
        entities.reserve(count);

        for (size_t i = 0; i < count; i++)
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

        auto it = mComponentStorage.find(typeIndex);
        if (it == mComponentStorage.end())
        {
            auto newSet = std::make_unique<SparseSet<Component>>();
            set = newSet.get();
            mComponentStorage[typeIndex] = std::move(newSet);
        }
        else
        {
            set = static_cast<SparseSet<Component>*>(it->second.get());
        }

        if (not set->Has(entity))
        {
            set->Insert(entity, component);
        }
        else
        {
            // TODO: log warning for adding already existing component
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

        auto it = mComponentStorage.find(typeIndex);

        if (it == mComponentStorage.end())
        {
            return;
        }

        auto* set = static_cast<SparseSet<Component>*>(it->second.get());

        if (set->Has(entity))
        {
            set->Remove(entity);
        }
        else
        {
            // TODO: log warning for component not existing on entity
        }
    }

    template <typename... Components>
    void Manager::ForEach(auto&& what)
    {
        auto sets = std::make_tuple(GetComponentSet<Components>()...);

        if (((std::get<SparseSet<Components>*>(sets) == nullptr) or ...))
        {
            return;
        }

        auto* primarySet = std::get<0>(sets);

        for (std::uint32_t i = 0; i < primarySet->Entities.size(); i++)
        {
            Entity e = primarySet->Entities[i];

            bool valid = ((std::get<SparseSet<Components>*>(sets)->Has(e)) and ...);

            if (not valid)
            {
                continue;
            }

            what(e, *std::get<SparseSet<Components>*>(sets)->Get(e)...);
        }
    }

    template <typename Component>
    SparseSet<Component>* Manager::GetComponentSet()
    {
        auto it = mComponentStorage.find(std::type_index(typeid(Component)));

        if (it == mComponentStorage.end())
        {
            return nullptr;
        }

        return static_cast<SparseSet<Component>*>(it->second.get());
    }

    template <typename Blueprint, std::size_t... Is>
    void Manager::AddComponentsImpl(Entity entity, std::index_sequence<Is...>)
    {
        (AddComponent<typename Blueprint::template ComponentType<Is>>(entity, {}), ...);
    }
}