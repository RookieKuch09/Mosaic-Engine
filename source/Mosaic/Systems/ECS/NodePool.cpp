#include <Mosaic/Systems/ECS/NodePool.hpp>

namespace Mosaic::Systems::ECS
{
    NodePool::NodePool()
        : mIndex(BLOCK_SIZE)
    {
    }

    NodePool::~NodePool()
    {
        for (auto* block : mBlocks)
        {
            delete[] block;
        }
    }

    Node* NodePool::Allocate()
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

    void NodePool::Deallocate(Node* ptr)
    {
        mFreeList.push_back(ptr);
    }

    void NodePool::AllocateBlock()
    {
        mBlocks.push_back(new Node[BLOCK_SIZE]);

        mIndex = 0;
    }
}