#include <Mosaic/Renderer/Component.hpp>
#include <Mosaic/Renderer/Renderer.hpp>

#include <Mosaic/Renderer/Backends/OpenGL.hpp>

#include <Mosaic/Application/Application.hpp>
#include <Mosaic/Application/Instance.hpp>

#include <Mosaic/ECS/EventManager.hpp>

namespace Mosaic
{
    Renderer::Renderer(InstanceResources& resources)
        : mBackend(nullptr), mInstanceResources(resources)
    {
        // TODO: allow for custom initial values for the renderer backend
        // TODO: switch backends based on parameter
        // TODO: set size based on window size
        mBackend = new OpenGLRendererBackend(resources, {800, 600}, {0.8, 0.4, 0.2, 1.0}, RendererVSyncMode::Disabled);

        mInstanceResources.EventManager.AddResponder(this, &Renderer::OnClearColourChangeRequest);
        mInstanceResources.EventManager.AddResponder(this, &Renderer::OnVSyncChangeRequest);
    }

    Renderer::~Renderer()
    {
        delete mBackend;
        mBackend = nullptr;
    }

    void Renderer::Setup()
    {
        mBackend->Setup();
    }

    void Renderer::Create()
    {
        mBackend->Create();
    }

    void Renderer::Update()
    {
        mBackend->Update();

        auto view = mInstanceResources.ECSManager.QueryView<RendererStateComponent>();

        for (auto [entity, component] : view)
        {
            component.ClearColour = mClearColour;
            component.VSyncMode = mVSyncMode;
        }
    }

    void Renderer::Destroy()
    {
        mBackend->Destroy();
    }

    void Renderer::OnClearColourChangeRequest(InstanceResources&, const RendererClearColourChangeRequest& request)
    {
        mClearColour = request.ClearColourRequest;

        mBackend->SetClearColour(mClearColour);
    }

    void Renderer::OnVSyncChangeRequest(InstanceResources&, const RendererVSyncModeChangeRequest& request)
    {
        mVSyncMode = request.VSyncModeRequest;

        mBackend->SetVSyncMode(mVSyncMode);
    }

    void Renderer::OnWindowResize(InstanceResources&, const WindowSizeChangeEvent& event)
    {
        mBackend->Resize(event.NewSize);
    }
}