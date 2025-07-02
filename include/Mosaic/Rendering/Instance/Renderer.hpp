#pragma once

#include <Mosaic/Rendering/Instance/Events.hpp>

#include <Mosaic/Macros/Exposure.hpp>

#include <glm/glm.hpp>

#include <type_traits>

namespace Mosaic
{
    enum class RendererVSyncMode
    {
        Strict,
        Relaxed,
        Disabled
    };

    class Application;
    class InstanceResources;
    class RendererBackend;

    struct WindowSizeChangeEvent;

    class MOSAIC_PUBLIC_EXPOSURE Renderer
    {
    public:
        Renderer(const Renderer& other) = delete;
        Renderer(Renderer&& other) noexcept = default;

        Renderer& operator=(const Renderer& other) = delete;
        Renderer& operator=(Renderer&& other) noexcept = delete;

    private:
        Renderer(InstanceResources& resources);
        ~Renderer();

        void Setup();
        void Create();
        void Update();
        void Destroy();

        void OnClearColourChangeRequest(InstanceResources&, const RendererClearColourChangeRequest& request);
        void OnVSyncChangeRequest(InstanceResources&, const RendererVSyncModeChangeRequest& request);
        void OnWindowResize(InstanceResources&, const WindowSizeChangeEvent& event);

        RendererBackend* mBackend;

        glm::fvec4 mClearColour;

        RendererVSyncMode mVSyncMode;

        InstanceResources& mInstanceResources;

        template <typename T> requires std::is_base_of_v<Application, T>
        friend class Instance;
    };
}