#include <mosaic/ecs/manager.hpp>

namespace Mosaic::ECS
{
    Manager::Manager(Resources& resources, Debug::Console& console)
        : mResources(&resources), mConsole(&console)
    {
    }

    auto Manager::CreateEntity() -> Entity
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

    auto Manager::CreateEntities(std::uint32_t count) -> std::vector<Entity>
    {
        std::vector<Entity> entities;

        entities.reserve(count);

        for (std::uint32_t i = 0; i < count; i++)
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
        for (const auto entity : entities)
        {
            DestroyEntity(entity);
        }
    }

    auto Manager::IsAlive(Entity entity) const -> bool
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
            system(*mResources);
        }
    }
}