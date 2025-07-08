#pragma once

#include <Mosaic/Application/Instance.hpp>

#include <Mosaic/Debug/Console/Resource.hpp>

#include <Mosaic/ECS/Entity.hpp>
#include <Mosaic/ECS/EntityManager.hpp>
#include <Mosaic/ECS/EntityView.hpp>

#include <boost/type_index.hpp>

namespace Mosaic
{
    template <typename Component>
    void EntityManager::AddComponent(EntityHandle entity, const Component& component)
    {
        auto typeIndex = std::type_index(typeid(Component));

        SparseSet<Component>* set = nullptr;

        if (!mComponentStorage.contains(typeIndex))
        {
            auto newSet = std::make_unique<SparseSet<Component>>();

            set = newSet.get();

            mComponentStorage[typeIndex] = std::move(newSet);
        }
        else
        {
            set = static_cast<SparseSet<Component>*>(mComponentStorage[typeIndex].get());
        }

        if (!set->Has(entity))
        {
            set->Insert(mInstanceResources.Console, entity, component);
        }
        else
        {
            auto name = boost::typeindex::type_id<Component>().pretty_name();

            // TODO: replace with log statement
        }
    }

    template <typename Component>
    void EntityManager::RemoveComponent(EntityHandle entity)
    {
        auto typeIndex = std::type_index(typeid(Component));

        if (!mComponentStorage.contains(typeIndex))
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

            // TODO: replace with log statement
        }
    }

    template <typename... Components>
    EntityView<Components...> EntityManager::QueryView()
    {
        return EntityView<Components...>(*this);
    }

    template <typename Component>
    SparseSet<Component>* EntityManager::GetComponentSet()
    {
        auto typeIndex = std::type_index(typeid(Component));

        if (!mComponentStorage.contains(typeIndex))
        {
            return nullptr;
        }

        return static_cast<SparseSet<Component>*>(mComponentStorage[typeIndex].get());
    }
}