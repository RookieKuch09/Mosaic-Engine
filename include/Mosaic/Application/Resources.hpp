#pragma once

#include <Mosaic/Macros/Exposure.hpp>

#include <type_traits>

namespace Mosaic
{
    class Application;

    class Console;
    class Window;
    class Renderer;

    class EntityManager;
    class EventManager;

    class MOSAIC_PUBLIC_EXPOSURE InstanceResources
    {
    public:
        InstanceResources(const InstanceResources& other) = delete;
        InstanceResources(InstanceResources&& other) noexcept = delete;

        InstanceResources& operator=(const InstanceResources& other) = delete;
        InstanceResources& operator=(InstanceResources&& other) noexcept = delete;

        Console& Console;
        Window& Window;
        Renderer& Renderer;

        EntityManager& EntityManager;
        EventManager& EventManager;

    private:
        InstanceResources(class Console& console, class Window& window, class Renderer& renderer, class EntityManager& ecsManager, class EventManager& eventManager);

        template <typename T> requires std::is_base_of_v<Application, T>
        friend class Instance;
    };
}