#pragma once

#include <Mosaic/Systems/ECS/EntityView.hpp>
#include <Mosaic/Systems/ECS/Registry.hpp>

namespace Mosaic::Systems::ECS
{
    template <typename... SupportedComponents>
    requires IsComponentTypes<ComponentTypes<SupportedComponents...>>
    EntityHandle Registry<ComponentTypes<SupportedComponents...>>::CreateEntity()
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
    requires IsComponentTypes<ComponentTypes<SupportedComponents...>>
    std::vector<EntityHandle> Registry<ComponentTypes<SupportedComponents...>>::CreateEntities(std::size_t count)
    {
        std::vector<EntityHandle> entities;
        entities.reserve(count);

        for (std::size_t i = 0; i < count; i++)
        {
            entities.push_back(CreateEntity());
        }

        return entities;
    }

    template <typename... SupportedComponents>
    requires IsComponentTypes<ComponentTypes<SupportedComponents...>>
    void Registry<ComponentTypes<SupportedComponents...>>::DestroyEntity(EntityHandle entity)
    {
        if (EntityExists(entity))
        {
            const Bitset& bitset = mEntityMasks[entity.ID];

            RemoveEntityFromSparseSets(entity, bitset);

            if (Archetype* archetype = mArchetypes.Get(bitset))
            {
                if (!archetype->Remove(entity.ID))
                {
                    Utilities::Terminal::LogWarning("EntityHandle(ID: {}, Generation: {}) does not exist in this archetype", entity.ID, entity.Generation);
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
            Utilities::Terminal::LogWarning("EntityHandle(ID: {}, Generation: {}) does not exist", entity.ID, entity.Generation);
        }
    }

    template <typename... SupportedComponents>
    requires IsComponentTypes<ComponentTypes<SupportedComponents...>>
    void Registry<ComponentTypes<SupportedComponents...>>::DestroyEntities(const std::vector<EntityHandle>& entities)
    {
        for (const auto& entity : entities)
        {
            DestroyEntity(entity);
        }
    }

    template <typename... SupportedComponents>
    requires IsComponentTypes<ComponentTypes<SupportedComponents...>>
    bool Registry<ComponentTypes<SupportedComponents...>>::EntityExists(EntityHandle entity) const
    {
        return entity.ID < mEntities.size() && mEntities[entity.ID].Generation == entity.Generation && mEntities[entity.ID].ID != std::numeric_limits<std::uint32_t>::max();
    }

    template <typename... SupportedComponents>
    requires IsComponentTypes<ComponentTypes<SupportedComponents...>>
    template <typename Component>
    requires(ComponentTypes<SupportedComponents...>::template Contains<Component>)
    bool Registry<ComponentTypes<SupportedComponents...>>::HasComponent(EntityHandle entity) const
    {
        if (EntityExists(entity))
        {
            constexpr std::size_t index = ComponentTypes<SupportedComponents...>::template Index<Component>();

            return mEntityMasks[entity.ID].test(index);
        }

        return false;
    }

    template <typename... SupportedComponents>
    requires IsComponentTypes<ComponentTypes<SupportedComponents...>>
    template <typename Component>
    requires(ComponentTypes<SupportedComponents...>::template Contains<Component>)
    void Registry<ComponentTypes<SupportedComponents...>>::AddComponent(EntityHandle entity, const Component& component)
    {
        if (EntityExists(entity))
        {
            auto& sparseSet = std::get<SparseSet<Component>>(mSparseSets);

            if (!sparseSet.Insert(entity.ID, component))
            {
                Utilities::Terminal::LogWarning("EntityHandle(ID: {}, Generation: {}) already has a component of type {}", entity.ID, entity.Generation, boost::typeindex::type_id<Component>().pretty_name());

                return;
            }

            constexpr std::size_t index = ComponentTypes<SupportedComponents...>::template Index<Component>();

            Bitset& bitset = mEntityMasks[entity.ID];
            Bitset oldBitset = bitset;

            bitset.set(index);

            UpdateArchetype(entity, oldBitset, bitset);
        }
        else
        {
            Utilities::Terminal::LogWarning("EntityHandle(ID: {}, Generation: {}) does not exist", entity.ID, entity.Generation);
        }
    }

    template <typename... SupportedComponents>
    requires IsComponentTypes<ComponentTypes<SupportedComponents...>>
    template <typename... Components>
    requires(ComponentTypes<Components...>::template Contains<Components> && ...)
    void Registry<ComponentTypes<SupportedComponents...>>::AddComponents(EntityHandle entity, const Components&... components)
    {
        if (EntityExists(entity))
        {
            (AddComponent<Components>(entity, components), ...);
        }
        else
        {
            Utilities::Terminal::LogWarning("EntityHandle(ID: {}, Generation: {}) does not exist", entity.ID, entity.Generation);
        }
    }

    template <typename... SupportedComponents>
    requires IsComponentTypes<ComponentTypes<SupportedComponents...>>
    template <typename Component>
    requires(ComponentTypes<SupportedComponents...>::template Contains<Component>)
    void Registry<ComponentTypes<SupportedComponents...>>::RemoveComponent(EntityHandle entity)
    {
        if (EntityExists(entity))
        {
            auto& sparseSet = std::get<SparseSet<Component>>(mSparseSets);

            if (!sparseSet.Remove(entity.ID))
            {
                Utilities::Terminal::LogWarning("EntityHandle(ID: {}, Generation: {}) has no component of type {}", entity.ID, entity.Generation, boost::typeindex::type_id<Component>().pretty_name());

                return;
            }

            constexpr std::size_t index = ComponentTypes<SupportedComponents...>::template Index<Component>();

            Bitset& bitset = mEntityMasks[entity.ID];
            Bitset oldBitset = bitset;

            bitset.reset(index);

            UpdateArchetype(entity, oldBitset, bitset);
        }
        else
        {
            Utilities::Terminal::LogWarning("EntityHandle(ID: {}, Generation: {}) does not exist", entity.ID, entity.Generation);
        }
    }

    template <typename... SupportedComponents>
    requires IsComponentTypes<ComponentTypes<SupportedComponents...>>
    template <typename... Components>
    requires(ComponentTypes<Components...>::template Contains<Components> && ...)
    void Registry<ComponentTypes<SupportedComponents...>>::RemoveComponents(EntityHandle entity)
    {
        if (EntityExists(entity))
        {
            (RemoveComponent<Components>(entity), ...);
        }
        else
        {
            Utilities::Terminal::LogWarning("EntityHandle(ID: {}, Generation: {}) does not exist", entity.ID, entity.Generation);
        }
    }

    template <typename... SupportedComponents>
    requires IsComponentTypes<ComponentTypes<SupportedComponents...>>
    [[nodiscard]] inline ArchetypeTree<sizeof...(SupportedComponents)>& Registry<ComponentTypes<SupportedComponents...>>::GetArchetypes()
    {
        return mArchetypes;
    }

    template <typename... SupportedComponents>
    requires IsComponentTypes<ComponentTypes<SupportedComponents...>>
    template <typename... Components>
    requires(ComponentTypes<SupportedComponents...>::template Contains<Components> && ...)
    constexpr Registry<ComponentTypes<SupportedComponents...>>::Bitset Registry<ComponentTypes<SupportedComponents...>>::GenerateBitset()
    {
        Bitset bitset;

        (bitset.set(ComponentTypes<SupportedComponents...>::template Index<Components>()), ...);

        return bitset;
    }

    template <typename... SupportedComponents>
    requires IsComponentTypes<ComponentTypes<SupportedComponents...>>
    template <typename... Components>
    requires(ComponentTypes<SupportedComponents...>::template Contains<Components> && ...)
    auto Registry<ComponentTypes<SupportedComponents...>>::TraverseView(SparseSet<EntityHandle>& entities)
    {
        return EntityView<Registry<ComponentTypes<SupportedComponents...>>, Components...>(this, entities);
    }

    template <typename... SupportedComponents>
    requires IsComponentTypes<ComponentTypes<SupportedComponents...>>
    template <typename Component>
    Component& Registry<ComponentTypes<SupportedComponents...>>::TrustedGetComponent(EntityHandle entity)
    {
        auto& sparseSet = std::get<SparseSet<Component>>(mSparseSets);

        return sparseSet.TrustedGetFromDense(entity.ID);
    }

    template <typename... SupportedComponents>
    requires IsComponentTypes<ComponentTypes<SupportedComponents...>>
    void Registry<ComponentTypes<SupportedComponents...>>::UpdateArchetype(EntityHandle entity, const Bitset& oldBitset, const Bitset& newBitset)
    {
        if (oldBitset == newBitset)
        {
            return;
        }

        if (Archetype* oldArchetype = mArchetypes.Get(oldBitset))
        {
            if (!oldArchetype->Remove(entity.ID))
            {
                Utilities::Terminal::LogWarning("EntityHandle(ID: {}, Generation: {}) does not exist in this archetype", entity.ID, entity.Generation);
            }

            if (oldArchetype->Size() == 0)
            {
                mArchetypes.Delete(oldBitset);
            }
        }

        if (!mArchetypes.GetOrCreate(newBitset).Insert(entity.ID, entity))
        {
            Utilities::Terminal::LogWarning("EntityHandle(ID: {}, Generation: {}) already exists in the new archetype", entity.ID, entity.Generation);
        }
    }

    template <typename... SupportedComponents>
    requires IsComponentTypes<ComponentTypes<SupportedComponents...>>
    template <std::size_t... Indices>
    void Registry<ComponentTypes<SupportedComponents...>>::RemoveEntityFromSparseSetsImpl(EntityHandle entity, const Bitset& mask, std::index_sequence<Indices...>)
    {
        ((mask.test(Indices) ? std::get<Indices>(mSparseSets).Remove(entity.ID) : void()), ...);
    }

    template <typename... SupportedComponents>
    requires IsComponentTypes<ComponentTypes<SupportedComponents...>>
    void Registry<ComponentTypes<SupportedComponents...>>::RemoveEntityFromSparseSets(EntityHandle entity, const Bitset& mask)
    {
        RemoveEntityFromSparseSetsImpl(entity, mask, std::index_sequence_for<SupportedComponents...>{});
    }
}