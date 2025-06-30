#pragma once

#include <Mosaic/Rendering/Backend.hpp>

namespace Mosaic
{
    class OpenGLRendererBackend : public RendererBackend
    {
    public:
        OpenGLRendererBackend(const OpenGLRendererBackend& other) = delete;
        OpenGLRendererBackend(OpenGLRendererBackend&& other) noexcept = delete;

        OpenGLRendererBackend& operator=(const OpenGLRendererBackend& other) = delete;
        OpenGLRendererBackend& operator=(OpenGLRendererBackend&& other) noexcept = delete;

    private:
        OpenGLRendererBackend(InstanceResources& instanceResources, glm::uvec2 size, glm::fvec4 clearColour, RendererVSyncMode vsyncMode);
        ~OpenGLRendererBackend() override = default;

        void Setup() override;
        void Create() override;
        void Update() override;
        void Destroy() override;

        void SetClearColour(glm::fvec4 clearColour) override;
        void SetVSyncMode(RendererVSyncMode vsyncMode) override;

        glm::fvec4 GetClearColour() override;
        RendererVSyncMode GetVSyncMode() override;

        void Resize(glm::uvec2 size) override;

        std::uint32_t GetSwapInterval();

        friend class Renderer;
    };
}