#pragma once

#include <Mosaic/Macros/Exposure.hpp>

#include <glm/glm.hpp>

#include <string>

namespace Mosaic
{
    class InstanceResources;

    enum class WindowVisibility : std::int32_t;

    class WindowBackend
    {
    public:
        WindowBackend(const WindowBackend& other) = delete;
        WindowBackend(WindowBackend&& other) noexcept = delete;

        WindowBackend& operator=(const WindowBackend& other) = delete;
        WindowBackend& operator=(WindowBackend&& other) noexcept = delete;

        virtual void GL_CreateContext(glm::uvec2 version) = 0;
        virtual void GL_ActivateContext() = 0;
        virtual void GL_SetSwapInterval(std::uint32_t interval) = 0;
        virtual void GL_SwapBuffers() = 0;

    protected:
        WindowBackend(InstanceResources& instanceResources, glm::uvec2 size, glm::uvec2 position, const std::string& title, WindowVisibility visibility);
        virtual ~WindowBackend() = default;

        virtual void Create() = 0;
        virtual void Update() = 0;
        virtual void Destroy() = 0;

        virtual void SetSize(glm::uvec2 size) = 0;
        virtual void SetPosition(glm::uvec2 position) = 0;
        virtual void SetTitle(const std::string& title) = 0;
        virtual void SetVisibility(WindowVisibility visibility) = 0;

        virtual glm::uvec2 GetSize() = 0;
        virtual glm::uvec2 GetPosition() = 0;
        virtual std::string GetTitle() = 0;
        virtual WindowVisibility GetVisibility() = 0;

        glm::uvec2 mSize;
        glm::uvec2 mPosition;

        std::string mTitle;

        WindowVisibility mVisibility;

        InstanceResources& mInstanceResources;

        friend class Window;
    };
}