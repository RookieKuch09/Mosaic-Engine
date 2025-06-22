#pragma once

#include <Mosaic/Application/Instance.hpp>

#include <Mosaic/Debug/Console.hpp>

#include <Mosaic/ECS/ECSManager.hpp>
#include <Mosaic/ECS/ECSView.hpp>
#include <Mosaic/ECS/Entity.hpp>

#include <boost/type_index.hpp>

namespace Mosaic
{
    template <typename Component>
    void ECSManager::AddComponent(Entity entity, const Component& component)
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
            set->Insert(mInstanceResources.Console, entity, component);
        }
        else
        {
            auto name = boost::typeindex::type_id<Component>().pretty_name();

            mInstanceResources.Console.Log<Console::LogSeverity::Warning>("Component of type {} already assigned to Entity {}", name, entity.ID);
        }
    }

    template <typename Component>
    void ECSManager::RemoveComponent(Entity entity)
    {
        auto typeIndex = std::type_index(typeid(Component));

        if (not mComponentStorage.contains(typeIndex))
        {
            return;
        }

        auto* set = static_cast<SparseSet<Component>*>(mComponentStorage[typeIndex].get());

        if (set->Has(entity))
        {
            set->Remove(mInstanceResources.Console, entity);
        }
        else
        {
            auto name = boost::typeindex::type_id<Component>().pretty_name();

            mInstanceResources.Console.Log<Console::LogSeverity::Warning>("Component of type {} is not associated with Entity {}", name, entity.ID);
        }
    }

    template <typename... Components>
    ECSView<Components...> ECSManager::QueryView()
    {
        return ECSView<Components...>(*this);
    }

    template <typename Component>
    SparseSet<Component>* ECSManager::GetComponentSet()
    {
        auto typeIndex = std::type_index(typeid(Component));

        if (not mComponentStorage.contains(typeIndex))
        {
            return nullptr;
        }

        return static_cast<SparseSet<Component>*>(mComponentStorage[typeIndex].get());
    }
}