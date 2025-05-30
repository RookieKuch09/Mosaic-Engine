#pragma once

#include <SDL3/SDL.h>

#include "application/events.hpp"
#include "utilities/numerics.hpp"
#include "utilities/vector.hpp"

#include <string>
#include <unordered_set>

namespace vk
{
    struct SurfaceKHR;
    struct Instance;
}

namespace Mosaic::Systems::Internal
{
    class Application;
    class EventManager;
}

namespace Mosaic::Systems::Internal::Rendering
{
    class Renderer;
    class VulkanRenderer;
    class VulkanSurface;
    class VulkanInstance;
    class OpenGLRenderer;
}

namespace Mosaic::Systems::Internal::Windowing
{
    struct ApplicationData;

    struct WindowMoveEvent
    {
        Types::Vec2<Types::UI32> Position;
    };

    struct WindowResizeEvent
    {
        Types::Vec2<Types::UI32> Size;
    };

    class Window
    {
    public:
        Window(Rendering::Renderer& renderer, EventManager& eventManager);
        ~Window();

        void SetConfigPath(const std::string& path);

    private:
        void Create();
        void Update();

        void Initialise();
        void CreateWindow();

        void QuitEvent();
        void ResizeEvent(const SDL_Event& event);
        void MoveEvent(const SDL_Event& event);

        void LoadConfig();

        void InitialiseVulkan();
        void GetVulkanWindowSurface(vk::SurfaceKHR& surface, vk::Instance& instance) const;
        std::unordered_set<std::string> GetVulkanRequiredInstanceExtensions() const;

        Types::Vec2<Types::UI32> mSize;
        Types::Vec2<Types::UI32> mPosition;

        std::string mTitle;
        std::string mConfigPath;

        bool mRunning;
        bool mFullscreen;
        bool mResizable;

        SDL_Window* mHandle;

        Rendering::Renderer& mRenderer;
        EventManager& mEventManager;

        friend class Mosaic::Internal::Application;
        friend class Mosaic::Internal::Rendering::VulkanRenderer;
        friend class Mosaic::Internal::Rendering::VulkanSurface;
        friend class Mosaic::Internal::Rendering::VulkanInstance;
        friend class Mosaic::Internal::Rendering::OpenGLRenderer;
    };
}