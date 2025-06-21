#include <Mosaic/ECS/ECSManager.hpp>

namespace Mosaic
{
    ECSManager::ECSManager(InstanceResources& resources)
        : mInstanceResources(resources)
    {
    }

    Entity ECSManager::CreateEntity()
    {
        EntityID entityID = 0;

        if (not mFreedIDs.empty())
        {
            entityID = mFreedIDs.back();
            mFreedIDs.pop_back();
        }
        else
        {
            entityID = static_cast<EntityID>(mGenerations.size());
            mGenerations.push_back(0);
        }

        return Entity{.ID = entityID, .Generation = mGenerations[entityID]};
    }

    std::vector<Entity> ECSManager::CreateEntities(std::uint32_t count)
    {
        std::vector<Entity> entities;

        entities.reserve(count);

        for (std::uint32_t i = 0; i < count; i++)
        {
            entities.push_back(CreateEntity());
        }

        return entities;
    }

    void ECSManager::DestroyEntity(Entity entity)
    {
        if (not EntityExists(entity))
        {
            return;
        }

        mGenerations[entity.ID]++;
        mFreedIDs.push_back(entity.ID);
    }

    void ECSManager::DestroyEntities(const std::vector<Entity>& entities)
    {
        for (const auto entity : entities)
        {
            DestroyEntity(entity);
        }
    }

    bool ECSManager::EntityExists(Entity entity) const
    {
        return entity.ID < mGenerations.size() and mGenerations[entity.ID] == entity.Generation;
    }

    void ECSManager::AddSystem(const ECSSystem& system)
    {
        mSystems.push_back(system);
    }

    void ECSManager::Update()
    {
        for (auto& system : mSystems)
        {
            system(mInstanceResources);
        }
    }
}