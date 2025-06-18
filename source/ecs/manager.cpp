#include <mosaic/ecs/manager.hpp>

namespace Mosaic
{
    ECSManager::ECSManager(Resources& resources)
        : mResources(resources)
    {
    }

    auto ECSManager::CreateEntity() -> Entity
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

    auto ECSManager::CreateEntities(std::uint32_t count) -> std::vector<Entity>
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
        if (not IsAlive(entity))
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

    auto ECSManager::IsAlive(Entity entity) const -> bool
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
            system(mResources);
        }
    }
}