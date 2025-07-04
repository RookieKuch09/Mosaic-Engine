#pragma once

#include <cstdint>
#include <functional>
#include <limits>

namespace Mosaic
{
    class ConsoleOutputHandle
    {
    public:
        static constexpr std::uint32_t INVALID_STATE = std::numeric_limits<std::uint32_t>::max();

        ConsoleOutputHandle() noexcept
            : mID(INVALID_STATE)
        {
        }

        ConsoleOutputHandle(const ConsoleOutputHandle& other)
        {
            if (other.mID != INVALID_STATE)
            {
                mID = other.mID;
            }
            else
            {
                // TODO: warn that invalid console output handles cannot be assigned
            }
        }

        ConsoleOutputHandle(ConsoleOutputHandle&& other) noexcept
        {
            if (other.mID != INVALID_STATE)
            {
                mID = other.mID;

                other.mID = INVALID_STATE;
            }
            else
            {
                // TODO: warn that invalid console output handles cannot be assigned
            }
        }

        ConsoleOutputHandle& operator=(const ConsoleOutputHandle& other)
        {
            if (other.mID != INVALID_STATE)
            {
                mID = other.mID;
            }
            else
            {
                // TODO: warn that invalid console output handles cannot be assigned
            }

            return *this;
        }

        ConsoleOutputHandle& operator=(ConsoleOutputHandle&& other) noexcept
        {
            if (other.mID != INVALID_STATE)
            {
                mID = other.mID;

                other.mID = INVALID_STATE;
            }
            else
            {
                // TODO: warn that invalid console output handles cannot be assigned
            }

            return *this;
        }

        friend bool operator==(const ConsoleOutputHandle& lhs, const ConsoleOutputHandle& rhs) noexcept
        {
            return lhs.mID == rhs.mID;
        }

        friend bool operator!=(const ConsoleOutputHandle& lhs, const ConsoleOutputHandle& rhs) noexcept
        {
            return !(lhs == rhs);
        }

    private:
        explicit ConsoleOutputHandle(std::uint32_t id) noexcept
            : mID(id)
        {
        }

        std::uint32_t mID;

        friend class Console;

        friend class std::hash<ConsoleOutputHandle>;
    };
}

namespace std
{
    template <>
    struct hash<Mosaic::ConsoleOutputHandle>
    {
        std::uint32_t operator()(const Mosaic::ConsoleOutputHandle& handle) const noexcept
        {
            return std::hash<std::uint32_t>{}(handle.mID);
        }
    };
}