#pragma once

#include <Mosaic/Systems/ECS/ArchetypeTree.hpp>
#include <Mosaic/Systems/ECS/ComponentTypes.hpp>
#include <Mosaic/Systems/ECS/Entity.hpp>
#include <Mosaic/Systems/ECS/SparseSet.hpp>

#include <Mosaic/Utilities/Terminal/Logging.hpp>

#include <bitset>
#include <tuple>
#include <vector>

#include <boost/type_index.hpp>

namespace Mosaic::Systems::ECS
{
    template <typename ComponentTypes>
    requires IsComponentTypes<ComponentTypes>
    class Registry;

    template <typename... SupportedComponents>
    requires IsComponentTypes<ComponentTypes<SupportedComponents...>>
    class Registry<ComponentTypes<SupportedComponents...>>
    {
    private:
        using Bitset = std::bitset<sizeof...(SupportedComponents)>;

    public:
        Registry() = default;
        ~Registry() = default;

        Registry(const Registry&) = delete;
        Registry(Registry&&) noexcept = delete;

        Registry& operator=(const Registry&) = delete;
        Registry& operator=(Registry&&) noexcept = delete;

        [[nodiscard]] inline EntityHandle CreateEntity();
        [[nodiscard]] inline std::vector<EntityHandle> CreateEntities(std::size_t count);

        inline void DestroyEntity(EntityHandle entity);
        inline void DestroyEntities(const std::vector<EntityHandle>& entities);

        [[nodiscard]] inline bool EntityExists(EntityHandle entity) const;

        template <typename Component>
        requires(ComponentTypes<SupportedComponents...>::template Contains<Component>)
        [[nodiscard]] inline bool HasComponent(EntityHandle entity) const;

        template <typename Component>
        requires(ComponentTypes<SupportedComponents...>::template Contains<Component>)
        inline void AddComponent(EntityHandle entity, const Component& component = Component());

        template <typename... Components>
        requires(ComponentTypes<Components...>::template Contains<Components> && ...)
        inline void AddComponents(EntityHandle entity, const Components&... components);

        template <typename Component>
        requires(ComponentTypes<SupportedComponents...>::template Contains<Component>)
        inline void RemoveComponent(EntityHandle entity);

        template <typename... Components>
        requires(ComponentTypes<Components...>::template Contains<Components> && ...)
        inline void RemoveComponents(EntityHandle entity);

        [[nodiscard]] inline ArchetypeTree<sizeof...(SupportedComponents)>& GetArchetypes();

        template <typename... Components>
        requires(ComponentTypes<SupportedComponents...>::template Contains<Components> && ...)
        [[nodiscard]] inline constexpr Bitset GenerateBitset();

        template <typename... Components>
        requires(ComponentTypes<SupportedComponents...>::template Contains<Components> && ...)
        [[nodiscard]] inline auto TraverseView(SparseSet<EntityHandle>& entities);

        template <typename Component>
        inline Component& TrustedGetComponent(EntityHandle entity);

    private:
        inline void UpdateArchetype(EntityHandle entity, const Bitset& oldBitset, const Bitset& newBitset);

        template <std::size_t... Indices>
        inline void RemoveEntityFromSparseSetsImpl(EntityHandle entity, const Bitset& mask, std::index_sequence<Indices...>);
        inline void RemoveEntityFromSparseSets(EntityHandle entity, const Bitset& mask);

        std::tuple<SparseSet<SupportedComponents>...> mSparseSets;

        ArchetypeTree<sizeof...(SupportedComponents)> mArchetypes;

        std::vector<Bitset> mEntityMasks;
        std::vector<EntityHandle> mEntities;
        std::vector<std::uint32_t> mFreeList;
    };
}

#include <Mosaic/Inline/Systems/ECS/Registry.inl>