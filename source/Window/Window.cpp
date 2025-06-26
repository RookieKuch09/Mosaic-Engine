#include <Mosaic/Window/Component.hpp>
#include <Mosaic/Window/Window.hpp>

#include <Mosaic/Window/Backends/GLFW.hpp>

#include <Mosaic/Application/Application.hpp>
#include <Mosaic/Application/Instance.hpp>

#include <Mosaic/ECS/EventManager.hpp>

namespace Mosaic
{
    Window::Window(InstanceResources& resources)
        : mBackend(nullptr), mInstanceResources(resources)
    {
        // TODO: allow for custom initial values for the window backend
        // TODO: switch backends based on macros || parameters
        mBackend = new GLFWWindowBackend(mInstanceResources, {800, 600}, {0, 0}, "Mosaic Window", WindowVisibility::Windowed);

        mInstanceResources.EventManager.AddResponder(this, &Window::OnWindowSizeChangeRequest);
        mInstanceResources.EventManager.AddResponder(this, &Window::OnWindowPositionChangeRequest);
        mInstanceResources.EventManager.AddResponder(this, &Window::OnWindowTitleChangeRequest);
        mInstanceResources.EventManager.AddResponder(this, &Window::OnWindowVisibilityChangeRequest);
    }

    Window::~Window()
    {
        delete mBackend;
        mBackend = nullptr;
    }

    void Window::Create()
    {
        mBackend->Create();
    }

    void Window::Update()
    {
        mBackend->Update();

        auto size = mBackend->GetSize();
        auto position = mBackend->GetPosition();
        auto title = mBackend->GetTitle();
        auto visibility = mBackend->GetVisibility();

        if (mSize != size)
        {
            mSize = size;

            mInstanceResources.EventManager.Emit<WindowSizeChangeEvent>(mSize);
        }

        if (mPosition != position)
        {
            mPosition = position;

            mInstanceResources.EventManager.Emit<WindowPositionChangeEvent>(mPosition);
        }

        if (mTitle != title)
        {
            mTitle = title;

            mInstanceResources.EventManager.Emit<WindowTitleChangeEvent>(mTitle);
        }

        if (mVisibility != visibility)
        {
            mVisibility = visibility;

            mInstanceResources.EventManager.Emit<WindowVisibilityChangeEvent>(mVisibility);
        }

        if (mVisibility == WindowVisibility::Destroy)
        {
            mInstanceResources.EventManager.Emit<ApplicationExitEvent>(0);
        }

        auto view = mInstanceResources.ECSManager.QueryView<WindowStateComponent>();

        for (auto [entity, component] : view)
        {
            component.Position = mPosition;
            component.Size = mSize;
            component.Title = mTitle;
            component.Visibility = mVisibility;
        }
    }

    void Window::Destroy()
    {
        mBackend->Destroy();
    }

    void Window::OnWindowSizeChangeRequest(InstanceResources&, const WindowSizeChangeRequest& request)
    {
        mSize = request.SizeRequest;

        mBackend->SetSize(mSize);
    }

    void Window::OnWindowPositionChangeRequest(InstanceResources&, const WindowPositionChangeRequest& request)
    {
        mPosition = request.PositionRequest;

        mBackend->SetPosition(mPosition);
    }

    void Window::OnWindowTitleChangeRequest(InstanceResources&, const WindowTitleChangeRequest& request)
    {
        mTitle = request.TitleRequest;

        mBackend->SetTitle(mTitle);
    }

    void Window::OnWindowVisibilityChangeRequest(InstanceResources&, const WindowVisibilityChangeRequest& request)
    {
        mVisibility = request.VisibilityRequest;

        mBackend->SetVisibility(mVisibility);
    }
}