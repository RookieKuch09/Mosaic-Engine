#pragma once

#include <bitset>
#include <optional>
#include <vector>

namespace Mosaic::Containers
{
    template <std::size_t BitCount, typename T>
    class BitsetTree
    {
    public:
        BitsetTree();
        ~BitsetTree() = default;

        BitsetTree(const BitsetTree&);
        BitsetTree(BitsetTree&&) noexcept;

        BitsetTree& operator=(const BitsetTree&);
        BitsetTree& operator=(BitsetTree&&) noexcept;

        void Delete(const std::bitset<BitCount>& bitset);

        [[nodiscard]] T& GetOrCreate(const std::bitset<BitCount>& bitset);

        [[nodiscard]] T* Get(const std::bitset<BitCount>& bitset);
        [[nodiscard]] const T* Get(const std::bitset<BitCount>& bitset) const;

        [[nodiscard]] auto begin();
        [[nodiscard]] auto end();

        [[nodiscard]] auto begin() const;
        [[nodiscard]] auto end() const;

    private:
        struct Node
        {
            Node* Zero = nullptr;
            Node* One = nullptr;

            std::optional<std::size_t> ArchetypeIndex;
        };

        class NodePool
        {
        public:
            NodePool();
            ~NodePool();

            NodePool(const NodePool&) = default;
            NodePool(NodePool&&) noexcept = default;

            NodePool& operator=(const NodePool&) = default;
            NodePool& operator=(NodePool&&) noexcept = default;

            [[nodiscard]] Node* Allocate();
            void Deallocate(Node* ptr);

        private:
            void AllocateBlock();

            static constexpr std::size_t BLOCK_SIZE = 1024;

            std::vector<Node*> mBlocks;
            std::vector<Node*> mFreeList;

            std::size_t mIndex;
        };

        Node* mRoot;
        NodePool mPool;

        std::vector<std::pair<std::bitset<BitCount>, T>> mContiguous;

        bool DeleteRecursive(Node*& node, const std::bitset<BitCount>& bitset, std::size_t depth);

        [[nodiscard]] Node* CloneSubtree(const Node* source, NodePool& pool);
    };

    template <typename>
    inline constexpr bool IsBitsetTree = false;

    template <std::size_t BitCount, typename T>
    inline constexpr bool IsBitsetTree<BitsetTree<BitCount, T>> = true;
}

#include <Mosaic/Inline/Containers/BitsetTree.inl>