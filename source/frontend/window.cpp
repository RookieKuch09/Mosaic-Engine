#include "../../include/frontend/window.hpp"
#include "../../include/frontend/contexts.hpp"

#include "../../include/backend/windows/glfw.hpp"

namespace Mosaic::Frontend
{
    WindowBackend::WindowBackend(GlobalContext& context, glm::uvec2& size, glm::uvec2& position, std::string& title)
        : EventLayer(context.mEventManager, context.mEventManager), mOwner(context.mWindow), mTitle(title), mSize(size), mPosition(position)
    {
    }

    Window::Window(GlobalContext& context)
        : mGlobalContext(context)
    {
        mBackendMapping = {
            {"GLFW", [this]()
             { return new Backend::Windows::GLFWWindow(mGlobalContext, mSize, mPosition, mTitle); }},
        };
    }

    void Window::Start()
    {
        auto& settingsFile = mGlobalContext.GetGlobalSettings();

        mTitle = settingsFile.Get<std::string>("Window.Title");
        auto size = settingsFile.Get<std::uint32_t, 2>("Window.Size");
        auto api = settingsFile.Get<std::string>("Window.API");

        mSize.x = size[0];
        mSize.y = size[1];

        if (not mBackendMapping.contains(api))
        {
            Utilities::Throw("Unsupported window API: {}", api);
        }

        mWindowBackend = mBackendMapping[api]();

        mWindowBackend->Start();
    }

    void Window::Update()
    {
        mWindowBackend->Update();
    }

    void Window::Stop()
    {
        mWindowBackend->Stop();
    }

    void Window::SetPosition(const glm::uvec2& position)
    {
        mPosition = position;

        mWindowBackend->SetPosition();
    }

    void Window::SetSize(const glm::uvec2& size)
    {
        mSize = size;

        mWindowBackend->SetSize();
    }

    void Window::SetTitle(const std::string& title)
    {
        mTitle = title;

        mWindowBackend->SetTitle();
    }

    glm::uvec2 Window::GetPosition()
    {
        mWindowBackend->GetPosition();

        return mPosition;
    }

    glm::uvec2 Window::GetSize()
    {
        mWindowBackend->GetSize();

        return mSize;
    }

    std::string Window::GetTitle()
    {
        mWindowBackend->GetTitle();

        return mTitle;
    }
}