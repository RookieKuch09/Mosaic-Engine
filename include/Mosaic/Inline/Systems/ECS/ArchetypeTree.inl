#pragma once

#include <Mosaic/Systems/ECS/ArchetypeTree.hpp>

namespace Mosaic::Systems::ECS
{
    template <std::size_t BitCount>
    ArchetypeTree<BitCount>::ArchetypeTree()
    {
        mRoot = mPool.Allocate();
        *mRoot = {};
    }

    template <std::size_t BitCount>
    void ArchetypeTree<BitCount>::Delete(const Bitset& bitset)
    {
        DeleteRecursive(mRoot, bitset, 0);
    }

    template <std::size_t BitCount>
    Archetype& ArchetypeTree<BitCount>::GetOrCreate(const Bitset& bitset)
    {
        Node* node = mRoot;

        for (std::size_t i = 0; i < BitCount; i++)
        {
            Node*& next = bitset[i] ? node->One : node->Zero;

            if (!next)
            {
                next = mPool.Allocate();
                *next = {};
            }

            node = next;
        }

        if (!node->ArchetypeIndex.has_value())
        {
            node->ArchetypeIndex = mArchetypes.size();
            mArchetypes.push_back(std::make_pair(bitset, Archetype()));
        }

        return mArchetypes[node->ArchetypeIndex.value()].second;
    }

    template <std::size_t BitCount>
    Archetype* ArchetypeTree<BitCount>::Get(const Bitset& bitset)
    {
        Node* node = mRoot;

        for (std::size_t i = 0; i < BitCount; i++)
        {
            node = bitset[i] ? node->One : node->Zero;
            if (!node)
            {
                return nullptr;
            }
        }

        if (!node->ArchetypeIndex.has_value())
        {
            return nullptr;
        }

        return &mArchetypes[node->ArchetypeIndex.value()].second;
    }

    template <std::size_t BitCount>
    const Archetype* ArchetypeTree<BitCount>::Get(const Bitset& bitset) const
    {
        Node* node = mRoot;

        for (std::size_t i = 0; i < BitCount; i++)
        {
            node = bitset[i] ? node->One : node->Zero;
            if (!node)
            {
                return nullptr;
            }
        }

        if (!node->ArchetypeIndex.has_value())
        {
            return nullptr;
        }

        return &mArchetypes[node->ArchetypeIndex.value()].second;
    }

    template <std::size_t BitCount>
    auto ArchetypeTree<BitCount>::begin()
    {
        return mArchetypes.begin();
    }

    template <std::size_t BitCount>
    auto ArchetypeTree<BitCount>::end()
    {
        return mArchetypes.end();
    }

    template <std::size_t BitCount>
    bool ArchetypeTree<BitCount>::DeleteRecursive(Node*& node, const Bitset& bitset, std::size_t depth)
    {
        if (!node)
        {
            return false;
        }

        if (depth == BitCount)
        {
            node->ArchetypeIndex.reset();

            return node->Zero == nullptr && node->One == nullptr;
        }

        Node*& next = bitset[depth] ? node->One : node->Zero;

        if (DeleteRecursive(next, bitset, depth + 1))
        {
            mPool.Deallocate(next);

            next = nullptr;
        }

        return node->ArchetypeIndex == std::nullopt && node->Zero == nullptr && node->One == nullptr;
    }
}