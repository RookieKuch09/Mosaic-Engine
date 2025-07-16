#pragma once

#include <cstdint>
#include <limits>
#include <optional>
#include <vector>

namespace Mosaic::Systems::ECS
{
    template <typename Element>
    class SparseSet
    {
    public:
        inline SparseSet() = default;
        inline ~SparseSet() = default;

        inline SparseSet(const SparseSet&) = default;
        inline SparseSet& operator=(const SparseSet&) = default;

        inline SparseSet(SparseSet&&) noexcept = default;
        inline SparseSet& operator=(SparseSet&&) noexcept = default;

        [[nodiscard]] inline bool Insert(std::uint32_t index, const Element& element);
        [[nodiscard]] inline bool Remove(std::uint32_t index);

        [[nodiscard]] inline std::optional<Element&> Get(std::uint32_t index);
        [[nodiscard]] inline std::optional<const Element&> Get(std::uint32_t index) const;

        [[nodiscard]] inline Element& TrustedGetFromDense(std::uint32_t index)
        {
            // We trust that the index is a dense mapping
            return mDense[index];
        }

        [[nodiscard]] inline const Element& TrustedGetFromDense(std::uint32_t index) const
        {
            // We trust that the index is a dense mapping
            return mDense[index];
        }

        [[nodiscard]] inline bool Has(std::uint32_t index) const;

        [[nodiscard]] inline auto begin()
        {
            return mDense.begin();
        }

        [[nodiscard]] inline auto begin() const
        {
            return mDense.begin();
        }

        [[nodiscard]] inline auto end()
        {
            return mDense.end();
        }

        [[nodiscard]] inline auto end() const
        {
            return mDense.end();
        }

        [[nodiscard]] inline std::size_t Size() const
        {
            return mDense.size();
        }

        [[nodiscard]] inline bool Empty() const
        {
            return mDense.empty();
        }

    private:
        static constexpr std::uint32_t DEAD_INDEX = std::numeric_limits<std::uint32_t>::max();

        std::vector<Element> mDense;
        std::vector<std::uint32_t> mSparse;
        std::vector<std::uint32_t> mDenseToEntity;
    };

    template <typename>
    inline constexpr bool IsSparseSet = false;

    template <typename Element>
    inline constexpr bool IsSparseSet<SparseSet<Element>> = true;
}

#include <Mosaic/Inline/Systems/ECS/SparseSet.inl>