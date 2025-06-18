#pragma once

#include <mosaic/application/resources.hpp>

#include <mosaic/debug/console.hpp>

#include <mosaic/ecs/entity.hpp>
#include <mosaic/ecs/manager.hpp>

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
            set->Insert(mResources.Console, entity, component);
        }
        else
        {
            auto name = boost::typeindex::type_id<Component>().pretty_name();

            mResources.Console.Log<Console::LogSeverity::Warning>("Component of type {} already assigned to Entity {}", name, entity.ID);
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
            set->Remove(mResources.Console, entity);
        }
        else
        {
            auto name = boost::typeindex::type_id<Component>().pretty_name();

            mResources.Console.Log<Console::LogSeverity::Warning>("Component of type {} is not associated with Entity {}", name, entity.ID);
        }
    }

    template <typename... Components>
    auto ECSManager::QueryView() -> ECSView<Components...>
    {
        return ECSView<Components...>(*this);
    }

    template <typename Component>
    auto ECSManager::GetComponentSet() -> SparseSet<Component>*
    {
        auto typeIndex = std::type_index(typeid(Component));

        if (not mComponentStorage.contains(typeIndex))
        {
            return nullptr;
        }

        return static_cast<SparseSet<Component>*>(mComponentStorage[typeIndex].get());
    }
}