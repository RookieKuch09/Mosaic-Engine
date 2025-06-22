#pragma once

#include <Mosaic/Window/Events.hpp>

#include <Mosaic/Macros/Exposure.hpp>

#include <glm/glm.hpp>

#include <string>
#include <type_traits>

namespace Mosaic
{
    enum class WindowVisibility
    {
        Minimised,
        Windowed,
        Fullscreen,
        Borderless,
        Destroy,
    };

    class Application;
    class InstanceResources;
    class WindowBackend;

    class MOSAIC_PUBLIC_EXPOSURE Window
    {
    public:
        Window(const Window& other) = delete;
        Window(Window&& other) noexcept = default;

        Window& operator=(const Window& other) = delete;
        Window& operator=(Window&& other) noexcept = delete;

    private:
        Window(InstanceResources& resources);
        ~Window();

        void Create();
        void Update();
        void Destroy();

        void OnWindowSizeChangeRequest(InstanceResources&, const WindowSizeChangeRequest& request);
        void OnWindowPositionChangeRequest(InstanceResources&, const WindowPositionChangeRequest& request);
        void OnWindowTitleChangeRequest(InstanceResources&, const WindowTitleChangeRequest& request);
        void OnWindowVisibilityChangeRequest(InstanceResources&, const WindowVisibilityChangeRequest& request);

        WindowBackend* mBackend;

        glm::uvec2 mSize;
        glm::uvec2 mPosition;

        std::string mTitle;

        WindowVisibility mVisibility;

        InstanceResources& mInstanceResources;

        template <typename T> requires std::is_base_of_v<Application, T>
        friend class Instance;

        friend class OpenGLRendererBackend;
    };
}