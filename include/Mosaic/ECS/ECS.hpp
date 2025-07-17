#pragma once

#include <Mosaic/Containers/BitsetTree.hpp>
#include <Mosaic/Containers/SparseSet.hpp>

#include <Mosaic/ECS/Archetype.hpp>
#include <Mosaic/ECS/ECSDescriptor.hpp>
#include <Mosaic/ECS/Entity.hpp>
#include <Mosaic/ECS/EntityView.hpp>

#include <Mosaic/Console/Console.hpp>

#include <bitset>
#include <tuple>
#include <vector>

#include <boost/type_index.hpp>

namespace Mosaic::ECS
{
    template <typename Descriptor>
    requires IsECSDescriptor<Descriptor>
    class ECS;

    template <typename... SupportedComponents>
    requires IsECSDescriptor<ECSDescriptor<SupportedComponents...>>
    class ECS<ECSDescriptor<SupportedComponents...>>
    {
    private:
        using Bitset = std::bitset<sizeof...(SupportedComponents)>;

    public:
        ECS(const Console::Console& console);
        ~ECS() = default;

        ECS(const ECS&) = delete;
        ECS(ECS&&) noexcept = delete;

        ECS& operator=(const ECS&) = delete;
        ECS& operator=(ECS&&) noexcept = delete;

        [[nodiscard]] inline Entity CreateEmptyEntity();
        [[nodiscard]] inline std::vector<Entity> CreateEmptyEntities(std::size_t count);

        template <typename... Components>
        requires(ECSDescriptor<SupportedComponents...>::template Contains<Components> && ...)
        inline Entity CreateEntity(Components&&... components);

        template <typename... Components>
        requires(ECSDescriptor<SupportedComponents...>::template Contains<Components> && ...)
        inline std::vector<Entity> CreateEntities(std::size_t count, Components&&... components);

        inline void DestroyEntity(Entity entity);
        inline void DestroyEntities(const std::vector<Entity>& entities);

        [[nodiscard]] inline bool EntityExists(Entity entity) const;

        template <typename Component>
        requires(ECSDescriptor<SupportedComponents...>::template Contains<Component>)
        [[nodiscard]] inline bool HasComponent(Entity entity) const;

        template <typename Component>
        requires(ECSDescriptor<SupportedComponents...>::template Contains<Component>)
        inline void AddComponent(Entity entity, const Component& component = Component());

        template <typename... Components>
        requires(ECSDescriptor<Components...>::template Contains<Components> && ...)
        inline void AddComponents(Entity entity, const Components&... components);

        template <typename Component>
        requires(ECSDescriptor<SupportedComponents...>::template Contains<Component>)
        inline void RemoveComponent(Entity entity);

        template <typename... Components>
        requires(ECSDescriptor<Components...>::template Contains<Components> && ...)
        inline void RemoveComponents(Entity entity);

        [[nodiscard]] inline Containers::BitsetTree<sizeof...(SupportedComponents), Archetype>& All();

        template <typename... Components>
        requires(ECSDescriptor<SupportedComponents...>::template Contains<Components> && ...)
        [[nodiscard]] inline constexpr Bitset Mask();

        template <typename... Components>
        requires(ECSDescriptor<SupportedComponents...>::template Contains<Components> && ...)
        [[nodiscard]] inline auto View(Archetype& archetype);

        template <typename Component>
        inline Component& UnsafeGetComponent(Entity entity);

    private:
        inline void UpdateArchetype(Entity entity, const Bitset& oldBitset, const Bitset& newBitset);

        template <std::size_t... Indices>
        inline void RemoveEntityFromSparseSetsImpl(Entity entity, const Bitset& mask, std::index_sequence<Indices...>);
        inline void RemoveEntityFromSparseSets(Entity entity, const Bitset& mask);

        std::tuple<Containers::SparseSet<SupportedComponents>...> mSparseSets;

        Containers::BitsetTree<sizeof...(SupportedComponents), Archetype> mArchetypes;

        std::vector<Bitset> mEntityMasks;
        std::vector<Entity> mEntities;
        std::vector<std::uint32_t> mFreeList;

        const Console::Console& mConsole;
    };
}

#include <Mosaic/Inline/ECS/ECS.inl>