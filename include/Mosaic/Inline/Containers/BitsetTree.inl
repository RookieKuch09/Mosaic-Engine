#pragma once
#pragma once

#include <Mosaic/Containers/BitsetTree.hpp>

namespace Mosaic::Containers
{
    template <std::size_t BitCount, typename T>
    BitsetTree<BitCount, T>::BitsetTree()
    {
        mRoot = mPool.Allocate();
        *mRoot = {};
    }

    template <std::size_t BitCount, typename T>
    BitsetTree<BitCount, T>::BitsetTree(const BitsetTree& other)
        : mContiguous(other.mContiguous)
    {
        mRoot = CloneSubtree(other.mRoot, mPool);
    }

    template <std::size_t BitCount, typename T>
    BitsetTree<BitCount, T>& BitsetTree<BitCount, T>::operator=(const BitsetTree& other)
    {
        if (this == &other)
        {
            return *this;
        }

        BitsetTree temp(other);

        std::swap(mRoot, temp.mRoot);
        std::swap(mPool, temp.mPool);
        std::swap(mContiguous, temp.mContiguous);

        return *this;
    }

    template <std::size_t BitCount, typename T>
    BitsetTree<BitCount, T>::BitsetTree(BitsetTree&& other) noexcept
        : mRoot(other.mRoot), mPool(std::move(other.mPool)), mContiguous(std::move(other.mContiguous))
    {
        other.mRoot = nullptr;
    }

    template <std::size_t BitCount, typename T>
    BitsetTree<BitCount, T>& BitsetTree<BitCount, T>::operator=(BitsetTree&& other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        Delete(std::bitset<BitCount>());

        mRoot = other.mRoot;
        mPool = std::move(other.mPool);
        mContiguous = std::move(other.mContiguous);

        other.mRoot = nullptr;

        return *this;
    }

    template <std::size_t BitCount, typename T>
    void BitsetTree<BitCount, T>::Delete(const std::bitset<BitCount>& bitset)
    {
        DeleteRecursive(mRoot, bitset, 0);
    }

    template <std::size_t BitCount, typename T>
    T& BitsetTree<BitCount, T>::GetOrCreate(const std::bitset<BitCount>& bitset)
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
            node->ArchetypeIndex = mContiguous.size();
            mContiguous.push_back(std::make_pair(bitset, T()));
        }

        return mContiguous[node->ArchetypeIndex.value()].second;
    }

    template <std::size_t BitCount, typename T>
    T* BitsetTree<BitCount, T>::Get(const std::bitset<BitCount>& bitset)
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

        return &mContiguous[node->ArchetypeIndex.value()].second;
    }

    template <std::size_t BitCount, typename T>
    const T* BitsetTree<BitCount, T>::Get(const std::bitset<BitCount>& bitset) const
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

        return &mContiguous[node->ArchetypeIndex.value()].second;
    }

    template <std::size_t BitCount, typename T>
    auto BitsetTree<BitCount, T>::begin()
    {
        return mContiguous.begin();
    }

    template <std::size_t BitCount, typename T>
    auto BitsetTree<BitCount, T>::end()
    {
        return mContiguous.end();
    }

    template <std::size_t BitCount, typename T>
    auto BitsetTree<BitCount, T>::begin() const
    {
        return mContiguous.begin();
    }

    template <std::size_t BitCount, typename T>
    auto BitsetTree<BitCount, T>::end() const
    {
        return mContiguous.end();
    }

    template <std::size_t BitCount, typename T>
    BitsetTree<BitCount, T>::NodePool::NodePool()
        : mIndex(BLOCK_SIZE)
    {
    }

    template <std::size_t BitCount, typename T>
    BitsetTree<BitCount, T>::NodePool::~NodePool()
    {
        for (auto* block : mBlocks)
        {
            delete[] block;
        }
    }

    template <std::size_t BitCount, typename T>
    BitsetTree<BitCount, T>::Node* BitsetTree<BitCount, T>::NodePool::Allocate()
    {
        if (!mFreeList.empty())
        {
            Node* node = mFreeList.back();

            mFreeList.pop_back();

            return node;
        }

        if (mIndex >= BLOCK_SIZE)
        {
            AllocateBlock();
        }

        return &mBlocks.back()[mIndex++];
    }

    template <std::size_t BitCount, typename T>
    void BitsetTree<BitCount, T>::NodePool::Deallocate(Node* ptr)
    {
        mFreeList.push_back(ptr);
    }

    template <std::size_t BitCount, typename T>
    void BitsetTree<BitCount, T>::NodePool::AllocateBlock()
    {
        mBlocks.push_back(new Node[BLOCK_SIZE]);

        mIndex = 0;
    }

    template <std::size_t BitCount, typename T>
    bool BitsetTree<BitCount, T>::DeleteRecursive(Node*& node, const std::bitset<BitCount>& bitset, std::size_t depth)
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

    template <std::size_t BitCount, typename T>
    BitsetTree<BitCount, T>::Node* BitsetTree<BitCount, T>::CloneSubtree(const Node* source, NodePool& pool)
    {
        if (!source)
        {
            return nullptr;
        }

        Node* clone = pool.Allocate();
        *clone = *source;

        clone->Zero = CloneSubtree(source->Zero, pool);
        clone->One = CloneSubtree(source->One, pool);

        return clone;
    }
}