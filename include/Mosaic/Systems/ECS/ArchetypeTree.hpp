#pragma once

#include <Mosaic/Systems/ECS/Archetype.hpp>
#include <Mosaic/Systems/ECS/NodePool.hpp>

#include <bitset>

namespace Mosaic::Systems::ECS
{
    template <std::size_t BitCount>
    class ArchetypeTree
    {
    public:
        using Bitset = std::bitset<BitCount>;

        ArchetypeTree();
        ~ArchetypeTree() = default;

        ArchetypeTree(const ArchetypeTree&) = delete;
        ArchetypeTree(ArchetypeTree&&) = delete;

        ArchetypeTree& operator=(const ArchetypeTree&) = delete;
        ArchetypeTree& operator=(ArchetypeTree&&) = delete;

        void Delete(const Bitset& bitset);

        [[nodiscard]] Archetype& GetOrCreate(const Bitset& bitset);

        [[nodiscard]] Archetype* Get(const Bitset& bitset);
        [[nodiscard]] const Archetype* Get(const Bitset& bitset) const;

        auto begin();
        auto end();

    private:
        Node* mRoot;
        NodePool mPool;

        std::vector<std::pair<std::bitset<BitCount>, Archetype>> mArchetypes;

        bool DeleteRecursive(Node*& node, const Bitset& bitset, std::size_t depth);
    };
}

#include <Mosaic/Inline/Systems/ECS/ArchetypeTree.inl>