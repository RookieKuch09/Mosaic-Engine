#pragma once

#include <cstdint>
#include <limits>
#include <optional>
#include <vector>

namespace Mosaic::Containers
{
    template <typename T>
    class SparseSet
    {
    public:
        inline SparseSet() = default;
        inline ~SparseSet() = default;

        inline SparseSet(const SparseSet&) = default;
        inline SparseSet& operator=(const SparseSet&) = default;

        inline SparseSet(SparseSet&&) noexcept = default;
        inline SparseSet& operator=(SparseSet&&) noexcept = default;

        [[nodiscard]] inline bool Insert(std::uint32_t index, const T& element);
        [[nodiscard]] inline bool Remove(std::uint32_t index);

        [[nodiscard]] inline std::optional<T&> Get(std::uint32_t index);
        [[nodiscard]] inline std::optional<const T&> Get(std::uint32_t index) const;

        [[nodiscard]] inline T& UnsafeGetFromDense(std::uint32_t index);
        [[nodiscard]] inline const T& UnsafeGetFromDense(std::uint32_t index) const;

        [[nodiscard]] inline T& UnsafeGetFromSparse(std::uint32_t index);
        [[nodiscard]] inline const T& UnsafeGetFromSparse(std::uint32_t index) const;

        [[nodiscard]] inline bool Has(std::uint32_t index) const;

        [[nodiscard]] inline auto begin();
        [[nodiscard]] inline auto begin() const;

        [[nodiscard]] inline auto end();
        [[nodiscard]] inline auto end() const;

        [[nodiscard]] inline std::size_t DenseSize() const;
        [[nodiscard]] inline std::size_t SparseSize() const;

        [[nodiscard]] inline bool Empty() const;

    private:
        static constexpr std::uint32_t DEAD_INDEX = std::numeric_limits<std::uint32_t>::max();

        std::vector<T> mDense;
        std::vector<std::uint32_t> mSparse;
        std::vector<std::uint32_t> mDenseToIndex;
    };

    template <typename>
    inline constexpr bool IsSparseSet = false;

    template <typename T>
    inline constexpr bool IsSparseSet<SparseSet<T>> = true;
}

#include <Mosaic/Inline/Containers/SparseSet.inl>