#pragma once

#include <Mosaic/Window/Backend.hpp>

namespace Mosaic
{
    enum class WindowVisibility : std::int32_t;

    class GLFWWindowBackend : public WindowBackend
    {
    public:
        GLFWWindowBackend(const GLFWWindowBackend& other) = delete;
        GLFWWindowBackend(GLFWWindowBackend&& other) noexcept = delete;

        GLFWWindowBackend& operator=(const GLFWWindowBackend& other) = delete;
        GLFWWindowBackend& operator=(GLFWWindowBackend&& other) noexcept = delete;

    protected:
        GLFWWindowBackend(InstanceResources& instanceResources, glm::uvec2 size, glm::uvec2 position, const std::string& title, WindowVisibility visibility);
        ~GLFWWindowBackend() override = default;

        void Create() override;
        void Update() override;
        void Destroy() override;

        void SetSize(glm::uvec2 size) override;
        void SetPosition(glm::uvec2 position) override;
        void SetTitle(const std::string& title) override;
        void SetVisibility(WindowVisibility visibility) override;

        glm::uvec2 GetSize() override;
        glm::uvec2 GetPosition() override;
        std::string GetTitle() override;
        WindowVisibility GetVisibility() override;

        void* mHandle;

        glm::ivec2 mWindowedPosition;
        glm::ivec2 mWindowedSize;

        friend class Window;
    };
}