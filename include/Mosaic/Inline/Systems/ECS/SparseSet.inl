#pragma once

#include <Mosaic/Systems/ECS/SparseSet.hpp>

#include <algorithm>

namespace Mosaic::Systems::ECS
{
    template <typename Component>
    inline bool SparseSet<Component>::Insert(std::uint32_t index, const Component& component)
    {
        // Append new index space if the new index is bigger than the sparse data
        if (index >= mSparse.size())
        {
            mSparse.resize(index + 1, DEAD_INDEX);
        }

        // Ensure the index doesn't exist already
        if (mSparse[index] != DEAD_INDEX)
        {
            return false;
        }

        // mDense.size() is the future index of our component
        mSparse[index] = mDense.size();

        mDense.push_back(component);
        mDenseToEntity.push_back(index);

        return true;
    }

    template <typename Component>
    inline bool SparseSet<Component>::Remove(std::uint32_t index)
    {
        // Check if the index is bigger than the sparse data
        if (index >= mSparse.size())
        {
            return false;
        }

        // Check if the index is dead
        if (mSparse[index] == DEAD_INDEX)
        {
            return false;
        }

        std::uint32_t lastIndex = static_cast<std::uint32_t>(mDense.size() - 1);
        std::uint32_t lastEntity = mDenseToEntity[lastIndex];
        std::uint32_t denseIndex = mSparse[index];

        // Swap the target and the last element
        if (denseIndex != lastIndex)
        {
            mDense[denseIndex] = std::move(mDense[lastIndex]);
            mDenseToEntity[denseIndex] = lastEntity;

            mSparse[lastEntity] = denseIndex;
        }

        mSparse[index] = DEAD_INDEX;

        mDense.pop_back();
        mDenseToEntity.pop_back();

        return true;
    }

    template <typename Component>
    inline std::optional<Component&> SparseSet<Component>::Get(std::uint32_t index)
    {
        // Check if the index is bigger than the sparse data
        if (index >= mSparse.size())
        {
            return std::nullopt;
        }

        // Check if the index is dead
        if (mSparse[index] == DEAD_INDEX)
        {
            return std::nullopt;
        }

        // We can guarantee that the index has a dense mapping
        return mDense[mSparse[index]];
    }

    template <typename Component>
    inline std::optional<const Component&> SparseSet<Component>::Get(std::uint32_t index) const
    {
        // Check if the index is bigger than the sparse data
        if (index >= mSparse.size())
        {
            return std::nullopt;
        }

        // Check if the index is dead
        if (mSparse[index] == DEAD_INDEX)
        {
            return std::nullopt;
        }

        // We can guarantee that the index has a dense mapping
        return mDense[mSparse[index]];
    }

    template <typename Component>
    inline bool SparseSet<Component>::Has(std::uint32_t index) const
    {
        // Check if the index is bigger than the sparse data
        if (index >= mSparse.size())
        {
            return false;
        }

        // Check if the index is dead
        if (mSparse[index] == DEAD_INDEX)
        {
            return false;
        }

        return true;
    }
}