#pragma once

#include <optional>
#include <vector>

namespace Mosaic::Systems::ECS
{
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

        NodePool(const NodePool&) = delete;
        NodePool(NodePool&&) noexcept = delete;

        NodePool& operator=(const NodePool&) = delete;
        NodePool& operator=(NodePool&&) noexcept = delete;

        [[nodiscard]] Node* Allocate();
        void Deallocate(Node* ptr);

    private:
        void AllocateBlock();

        static constexpr std::size_t BLOCK_SIZE = 1024;

        std::vector<Node*> mBlocks;
        std::vector<Node*> mFreeList;

        std::size_t mIndex;
    };

}