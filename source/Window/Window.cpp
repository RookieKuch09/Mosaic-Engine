#include <Mosaic/Window/Window.hpp>

#include <Mosaic/Application/Application.hpp>
#include <Mosaic/Application/Instance.hpp>

#include <Mosaic/ECS/EventManager.hpp>

namespace Mosaic
{
    WindowResources& Window::GetResources()
    {
        return mWindowResources;
    }

    const WindowResources& Window::GetResources() const
    {
        return mWindowResources;
    }

    Window::Window(InstanceResources& resources)
        : mBackend(mWindowResources, resources), mInstanceResources(resources)
    {
        mBackend.Create();
    }

    Window::~Window()
    {
        mBackend.Destroy();
    }

    void Window::Setup()
    {
        mWindowResourcesCopy = mWindowResources;

        mBackend.SetState();
    }

    void Window::Update()
    {
        mBackend.Update();

        mBackend.GetState();

        if (mWindowResources.Size != mWindowResourcesCopy.Size)
        {
            mWindowResourcesCopy.Size = mWindowResources.Size;

            mBackend.SetSize(mWindowResources.Size);

            mInstanceResources.EventManager.Emit<WindowSizeChangeEvent>(mWindowResources.Size);
        }

        if (mWindowResources.Position != mWindowResourcesCopy.Position)
        {
            mWindowResourcesCopy.Position = mWindowResources.Position;

            mBackend.SetPosition(mWindowResources.Position);

            mInstanceResources.EventManager.Emit<WindowPositionChangeEvent>(mWindowResources.Position);
        }

        if (mWindowResources.Title != mWindowResourcesCopy.Title)
        {
            mWindowResourcesCopy.Title = mWindowResources.Title;

            mBackend.SetTitle(mWindowResources.Title);

            mInstanceResources.EventManager.Emit<WindowTitleChangeEvent>(mWindowResources.Title);
        }

        if (mWindowResources.Visibility != mWindowResourcesCopy.Visibility)
        {
            mWindowResourcesCopy.Visibility = mWindowResources.Visibility;

            mBackend.SetVisibility(mWindowResources.Visibility);

            mInstanceResources.EventManager.Emit<WindowVisibilityChangeEvent>(mWindowResources.Visibility);
        }

        if (mWindowResources.Visibility == WindowVisibility::Destroy)
        {
            mInstanceResources.EventManager.Emit<ApplicationExitEvent>(0);
        }
    }
}