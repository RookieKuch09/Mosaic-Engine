#pragma once

#include <Mosaic/ECS/ECS.hpp>

namespace Mosaic::ECS
{
    template <typename... SupportedComponents>
    requires IsECSDescriptor<ECSDescriptor<SupportedComponents...>>
    ECS<ECSDescriptor<SupportedComponents...>>::ECS(const Console::Console& console)
        : mConsole(console)
    {
    }

    template <typename... SupportedComponents>
    requires IsECSDescriptor<ECSDescriptor<SupportedComponents...>>
    Entity ECS<ECSDescriptor<SupportedComponents...>>::CreateEmptyEntity()
    {
        if (mFreeList.empty())
        {
            mEntities.push_back({static_cast<std::uint32_t>(mEntities.size()), 0});
            mEntityMasks.push_back(Bitset{});

            return mEntities.back();
        }
        else
        {
            std::uint32_t index = mFreeList.back();

            mFreeList.pop_back();
            mEntities[index].Generation++;

            return mEntities[index];
        }
    }

    template <typename... SupportedComponents>
    requires IsECSDescriptor<ECSDescriptor<SupportedComponents...>>
    std::vector<Entity> ECS<ECSDescriptor<SupportedComponents...>>::CreateEmptyEntities(std::size_t count)
    {
        std::vector<Entity> entities;
        entities.reserve(count);

        for (std::size_t i = 0; i < count; i++)
        {
            entities.push_back(CreateEntity());
        }

        return entities;
    }

    template <typename... SupportedComponents>
    requires IsECSDescriptor<ECSDescriptor<SupportedComponents...>>
    template <typename... Components>
    requires(ECSDescriptor<SupportedComponents...>::template Contains<Components> && ...)
    Entity ECS<ECSDescriptor<SupportedComponents...>>::CreateEntity(Components&&... components)
    {
        Entity entity = CreateEmptyEntity();

        AddComponents<Components...>(entity, std::forward<Components>(components)...);

        return entity;
    }

    template <typename... SupportedComponents>
    requires IsECSDescriptor<ECSDescriptor<SupportedComponents...>>
    template <typename... Components>
    requires(ECSDescriptor<SupportedComponents...>::template Contains<Components> && ...)
    std::vector<Entity> ECS<ECSDescriptor<SupportedComponents...>>::CreateEntities(std::size_t count, Components&&... components)
    {
        std::vector<Entity> entities;
        entities.reserve(count);

        for (std::size_t i = 0; i < count; i++)
        {
            entities.push_back(CreateEntity(std::forward<Components>(components)...));
        }

        return entities;
    }

    template <typename... SupportedComponents>
    requires IsECSDescriptor<ECSDescriptor<SupportedComponents...>>
    void ECS<ECSDescriptor<SupportedComponents...>>::DestroyEntity(Entity entity)
    {
        if (EntityExists(entity))
        {
            const Bitset& bitset = mEntityMasks[entity.ID];

            RemoveEntityFromSparseSets(entity, bitset);

            if (Archetype* archetype = mArchetypes.Get(bitset))
            {
                if (!archetype->Remove(entity.ID))
                {
                    mConsole.LogWarning("Entity(ID: {}, Generation: {}) does not exist in this archetype", entity.ID, entity.Generation);
                }

                if (archetype->Size() == 0)
                {
                    mArchetypes.Delete(bitset);
                }
            }

            mFreeList.push_back(entity.ID);
            mEntities[entity.ID] = {std::numeric_limits<std::uint32_t>::max(), entity.Generation};
            mEntityMasks[entity.ID].reset();
        }
        else
        {
            mConsole.LogWarning("Entity(ID: {}, Generation: {}) does not exist", entity.ID, entity.Generation);
        }
    }

    template <typename... SupportedComponents>
    requires IsECSDescriptor<ECSDescriptor<SupportedComponents...>>
    void ECS<ECSDescriptor<SupportedComponents...>>::DestroyEntities(const std::vector<Entity>& entities)
    {
        for (const auto& entity : entities)
        {
            DestroyEntity(entity);
        }
    }

    template <typename... SupportedComponents>
    requires IsECSDescriptor<ECSDescriptor<SupportedComponents...>>
    bool ECS<ECSDescriptor<SupportedComponents...>>::EntityExists(Entity entity) const
    {
        return entity.ID < mEntities.size() && mEntities[entity.ID].Generation == entity.Generation && mEntities[entity.ID].ID != std::numeric_limits<std::uint32_t>::max();
    }

    template <typename... SupportedComponents>
    requires IsECSDescriptor<ECSDescriptor<SupportedComponents...>>
    template <typename Component>
    requires(ECSDescriptor<SupportedComponents...>::template Contains<Component>)
    bool ECS<ECSDescriptor<SupportedComponents...>>::HasComponent(Entity entity) const
    {
        if (EntityExists(entity))
        {
            constexpr std::size_t index = ECSDescriptor<SupportedComponents...>::template Index<Component>();

            return mEntityMasks[entity.ID].test(index);
        }

        return false;
    }

    template <typename... SupportedComponents>
    requires IsECSDescriptor<ECSDescriptor<SupportedComponents...>>
    template <typename Component>
    requires(ECSDescriptor<SupportedComponents...>::template Contains<Component>)
    void ECS<ECSDescriptor<SupportedComponents...>>::AddComponent(Entity entity, const Component& component)
    {
        if (EntityExists(entity))
        {
            auto& sparseSet = std::get<Containers::SparseSet<Component>>(mSparseSets);

            if (!sparseSet.Insert(entity.ID, component))
            {
                mConsole.LogWarning("Entity(ID: {}, Generation: {}) already has a component of type {}", entity.ID, entity.Generation, Types::Reflection::TypeInfo<Component>::Name);

                return;
            }

            constexpr std::size_t index = ECSDescriptor<SupportedComponents...>::template Index<Component>();

            Bitset& bitset = mEntityMasks[entity.ID];
            Bitset oldBitset = bitset;

            bitset.set(index);

            UpdateArchetype(entity, oldBitset, bitset);
        }
        else
        {
            mConsole.LogWarning("Entity(ID: {}, Generation: {}) does not exist", entity.ID, entity.Generation);
        }
    }

    template <typename... SupportedComponents>
    requires IsECSDescriptor<ECSDescriptor<SupportedComponents...>>
    template <typename... Components>
    requires(ECSDescriptor<Components...>::template Contains<Components> && ...)
    void ECS<ECSDescriptor<SupportedComponents...>>::AddComponents(Entity entity, const Components&... components)
    {
        if (EntityExists(entity))
        {
            (AddComponent<Components>(entity, components), ...);
        }
        else
        {
            mConsole.LogWarning("Entity(ID: {}, Generation: {}) does not exist", entity.ID, entity.Generation);
        }
    }

    template <typename... SupportedComponents>
    requires IsECSDescriptor<ECSDescriptor<SupportedComponents...>>
    template <typename Component>
    requires(ECSDescriptor<SupportedComponents...>::template Contains<Component>)
    void ECS<ECSDescriptor<SupportedComponents...>>::RemoveComponent(Entity entity)
    {
        if (EntityExists(entity))
        {
            auto& sparseSet = std::get<Containers::SparseSet<Component>>(mSparseSets);

            if (!sparseSet.Remove(entity.ID))
            {
                mConsole.LogWarning("Entity(ID: {}, Generation: {}) has no component of type {}", entity.ID, entity.Generation, Types::Reflection::TypeInfo<Component>::Name);

                return;
            }

            constexpr std::size_t index = ECSDescriptor<SupportedComponents...>::template Index<Component>();

            Bitset& bitset = mEntityMasks[entity.ID];
            Bitset oldBitset = bitset;

            bitset.reset(index);

            UpdateArchetype(entity, oldBitset, bitset);
        }
        else
        {
            mConsole.LogWarning("Entity(ID: {}, Generation: {}) does not exist", entity.ID, entity.Generation);
        }
    }

    template <typename... SupportedComponents>
    requires IsECSDescriptor<ECSDescriptor<SupportedComponents...>>
    template <typename... Components>
    requires(ECSDescriptor<Components...>::template Contains<Components> && ...)
    void ECS<ECSDescriptor<SupportedComponents...>>::RemoveComponents(Entity entity)
    {
        if (EntityExists(entity))
        {
            (RemoveComponent<Components>(entity), ...);
        }
        else
        {
            mConsole.LogWarning("Entity(ID: {}, Generation: {}) does not exist", entity.ID, entity.Generation);
        }
    }

    template <typename... SupportedComponents>
    requires IsECSDescriptor<ECSDescriptor<SupportedComponents...>>
    [[nodiscard]] inline Containers::BitsetTree<sizeof...(SupportedComponents), Archetype>& ECS<ECSDescriptor<SupportedComponents...>>::All()
    {
        return mArchetypes;
    }

    template <typename... SupportedComponents>
    requires IsECSDescriptor<ECSDescriptor<SupportedComponents...>>
    template <typename... Components>
    requires(ECSDescriptor<SupportedComponents...>::template Contains<Components> && ...)
    constexpr ECS<ECSDescriptor<SupportedComponents...>>::Bitset ECS<ECSDescriptor<SupportedComponents...>>::Mask()
    {
        Bitset bitset;

        (bitset.set(ECSDescriptor<SupportedComponents...>::template Index<Components>()), ...);

        return bitset;
    }

    template <typename... SupportedComponents>
    requires IsECSDescriptor<ECSDescriptor<SupportedComponents...>>
    template <typename... Components>
    requires(ECSDescriptor<SupportedComponents...>::template Contains<Components> && ...)
    auto ECS<ECSDescriptor<SupportedComponents...>>::View(Archetype& archetype)
    {
        return EntityView<ECS<ECSDescriptor<SupportedComponents...>>, Components...>(this, archetype.GetEntities());
    }

    template <typename... SupportedComponents>
    requires IsECSDescriptor<ECSDescriptor<SupportedComponents...>>
    template <typename Component>
    Component& ECS<ECSDescriptor<SupportedComponents...>>::UnsafeGetComponent(Entity entity)
    {
        auto& sparseSet = std::get<Containers::SparseSet<Component>>(mSparseSets);

        return sparseSet.UnsafeGetFromSparse(entity.ID);
    }

    template <typename... SupportedComponents>
    requires IsECSDescriptor<ECSDescriptor<SupportedComponents...>>
    void ECS<ECSDescriptor<SupportedComponents...>>::UpdateArchetype(Entity entity, const Bitset& oldBitset, const Bitset& newBitset)
    {
        if (oldBitset == newBitset)
        {
            return;
        }

        if (Archetype* oldArchetype = mArchetypes.Get(oldBitset))
        {
            if (!oldArchetype->Remove(entity.ID))
            {
                mConsole.LogWarning("Entity(ID: {}, Generation: {}) does not exist in this archetype", entity.ID, entity.Generation);
            }

            if (oldArchetype->Size() == 0)
            {
                mArchetypes.Delete(oldBitset);
            }
        }

        if (!mArchetypes.GetOrCreate(newBitset).Insert(entity.ID, entity))
        {
            mConsole.LogWarning("Entity(ID: {}, Generation: {}) already exists in the new archetype", entity.ID, entity.Generation);
        }
    }

    template <typename... SupportedComponents>
    requires IsECSDescriptor<ECSDescriptor<SupportedComponents...>>
    template <std::size_t... Indices>
    void ECS<ECSDescriptor<SupportedComponents...>>::RemoveEntityFromSparseSetsImpl(Entity entity, const Bitset& mask, std::index_sequence<Indices...>)
    {
        ((mask.test(Indices) ? std::get<Indices>(mSparseSets).Remove(entity.ID) : void()), ...);
    }

    template <typename... SupportedComponents>
    requires IsECSDescriptor<ECSDescriptor<SupportedComponents...>>
    void ECS<ECSDescriptor<SupportedComponents...>>::RemoveEntityFromSparseSets(Entity entity, const Bitset& mask)
    {
        RemoveEntityFromSparseSetsImpl(entity, mask, std::index_sequence_for<SupportedComponents...>{});
    }
}