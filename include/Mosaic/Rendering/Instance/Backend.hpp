#pragma once

#include <Mosaic/Macros/Exposure.hpp>

#include <glm/glm.hpp>

namespace Mosaic
{
    class InstanceResources;

    enum class RendererVSyncMode : std::int32_t;

    class RendererBackend
    {
    public:
        RendererBackend(const RendererBackend& other) = delete;
        RendererBackend(RendererBackend&& other) noexcept = delete;

        RendererBackend& operator=(const RendererBackend& other) = delete;
        RendererBackend& operator=(RendererBackend&& other) noexcept = delete;

    protected:
        RendererBackend(InstanceResources& instanceResources, glm::uvec2 size, glm::fvec4 clearColour, RendererVSyncMode vsyncMode);
        virtual ~RendererBackend() = default;

        virtual void Setup() = 0;
        virtual void Create() = 0;
        virtual void Update() = 0;
        virtual void Destroy() = 0;

        virtual void SetClearColour(glm::fvec4 clearColour) = 0;
        virtual void SetVSyncMode(RendererVSyncMode vsyncMode) = 0;

        virtual glm::fvec4 GetClearColour() = 0;
        virtual RendererVSyncMode GetVSyncMode() = 0;

        virtual void Resize(glm::uvec2 size) = 0;

        glm::uvec2 mSize;
        glm::fvec4 mClearColour;

        RendererVSyncMode mVSyncMode;

        InstanceResources& mInstanceResources;

        friend class Renderer;
    };
}