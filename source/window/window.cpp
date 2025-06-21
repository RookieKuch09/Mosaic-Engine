#include <mosaic/window/window.hpp>

#include <mosaic/application/application.hpp>
#include <mosaic/application/resources.hpp>

#include <mosaic/events/manager.hpp>

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

    Window::Window(ApplicationResources& resources)
        : mBackend(mWindowResources, resources), mApplicationResources(resources)
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

            mApplicationResources.EventManager.Emit<WindowSizeChangeEvent>(mWindowResources.Size);
        }

        if (mWindowResources.Position != mWindowResourcesCopy.Position)
        {
            mWindowResourcesCopy.Position = mWindowResources.Position;

            mBackend.SetPosition(mWindowResources.Position);

            mApplicationResources.EventManager.Emit<WindowPositionChangeEvent>(mWindowResources.Position);
        }

        if (mWindowResources.Title != mWindowResourcesCopy.Title)
        {
            mWindowResourcesCopy.Title = mWindowResources.Title;

            mBackend.SetTitle(mWindowResources.Title);

            mApplicationResources.EventManager.Emit<WindowTitleChangeEvent>(mWindowResources.Title);
        }

        if (mWindowResources.Visibility != mWindowResourcesCopy.Visibility)
        {
            mWindowResourcesCopy.Visibility = mWindowResources.Visibility;

            mBackend.SetVisibility(mWindowResources.Visibility);

            mApplicationResources.EventManager.Emit<WindowVisibilityChangeEvent>(mWindowResources.Visibility);
        }

        if (mWindowResources.Visibility == WindowVisibility::Destroy)
        {
            mApplicationResources.EventManager.Emit<ApplicationExitEvent>(0);
        }
    }
}