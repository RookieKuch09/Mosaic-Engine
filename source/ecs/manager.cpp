#include <mosaic/ecs/manager.hpp>

namespace Mosaic::ECS
{
    Manager::Manager(Resources& resources)
        : mResources(resources)
    {
    }

    Entity Manager::CreateEntity()
    {
        EntityID id;

        if (not mFreedIDs.empty())
        {
            id = mFreedIDs.back();
            mFreedIDs.pop_back();
        }
        else
        {
            id = static_cast<EntityID>(mGenerations.size());
            mGenerations.push_back(0);
        }

        return Entity{id, mGenerations[id]};
    }

    std::vector<Entity> Manager::CreateEntities(size_t count)
    {
        std::vector<Entity> entities;
        entities.reserve(count);

        for (size_t i = 0; i < count; i++)
        {
            entities.push_back(CreateEntity());
        }

        return entities;
    }

    void Manager::DestroyEntity(Entity entity)
    {
        if (not IsAlive(entity))
        {
            return;
        }

        mGenerations[entity.ID]++;
        mFreedIDs.push_back(entity.ID);
    }

    void Manager::DestroyEntities(const std::vector<Entity>& entities)
    {
        for (auto& entity : entities)
        {
            DestroyEntity(entity);
        }
    }

    bool Manager::IsAlive(Entity entity) const
    {
        return entity.ID < mGenerations.size() and mGenerations[entity.ID] == entity.Generation;
    }

    void Manager::AddSystem(const System& system)
    {
        mSystems.push_back(system);
    }

    void Manager::Update()
    {
        for (auto& system : mSystems)
        {
            system(mResources);
        }
    }
}