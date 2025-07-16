#pragma once

#include <array>
#include <limits>
#include <type_traits>

namespace Mosaic::Systems::ECS
{
    template <typename Component>
    inline constexpr bool CompatibleComponent = !std::is_pointer_v<Component> &&
                                                !std::is_reference_v<Component> &&
                                                !std::is_const_v<Component> &&
                                                !std::is_volatile_v<Component>;

    template <typename...>
    inline constexpr bool HasDuplicateTypes = false;

    template <typename First, typename... Rest>
    inline constexpr bool HasDuplicateTypes<First, Rest...> = (std::is_same_v<First, Rest> || ...) || HasDuplicateTypes<Rest...>;

    template <typename... Components>
    requires((CompatibleComponent<Components> && ...) && !HasDuplicateTypes<Components...>)
    class ComponentTypes
    {
    public:
        ComponentTypes() = delete;

        template <typename T>
        static constexpr bool Contains = (std::is_same_v<T, Components> || ...);

        template <typename T>
        requires Contains<T>
        static consteval std::size_t Index()
        {
            constexpr std::array<bool, sizeof...(Components)> matchFlags = {std::is_same_v<T, Components>...};

            for (std::size_t i = 0; i < matchFlags.size(); i++)
            {
                if (matchFlags[i])
                {
                    return i;
                }
            }

            return std::numeric_limits<std::size_t>::max();
        }
    };

    template <typename>
    inline constexpr bool IsComponentTypes = false;

    template <typename... Ts>
    inline constexpr bool IsComponentTypes<ComponentTypes<Ts...>> = true;
}