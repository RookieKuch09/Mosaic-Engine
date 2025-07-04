#include <Mosaic/ECS/EntityManager.hpp>

namespace Mosaic
{
    EntityManager::EntityManager(InstanceResources& resources)
        : mInstanceResources(resources)
    {
    }

    Entity EntityManager::CreateEntity()
    {
        EntityHandle entityHandle = 0;

        if (!mFreedIDs.empty())
        {
            entityHandle = mFreedIDs.back();
            mFreedIDs.pop_back();
        }
        else
        {
            entityHandle = mGenerations.size();
            mGenerations.push_back(0);
        }

        return Entity{.Handle = entityHandle, .Generation = mGenerations[entityHandle]};
    }

    std::vector<Entity> EntityManager::CreateEntities(std::uint32_t count)
    {
        std::vector<Entity> entities;

        entities.reserve(count);

        for (std::uint32_t i = 0; i < count; i++)
        {
            entities.push_back(CreateEntity());
        }

        return entities;
    }

    void EntityManager::DestroyEntity(Entity entity)
    {
        if (!EntityExists(entity))
        {
            return;
        }

        mGenerations[entity.Handle]++;
        mFreedIDs.push_back(entity.Handle);
    }

    void EntityManager::DestroyEntities(const std::vector<Entity>& entities)
    {
        for (const auto entity : entities)
        {
            DestroyEntity(entity);
        }
    }

    bool EntityManager::EntityExists(Entity entity) const
    {
        return entity.Handle < mGenerations.size() && mGenerations[entity.Handle] == entity.Generation;
    }

    void EntityManager::AddSystem(const EntitySystem& system)
    {
        mSystems.push_back(system);
    }

    void EntityManager::Update()
    {
        for (auto& system : mSystems)
        {
            system(mInstanceResources);
        }
    }
}