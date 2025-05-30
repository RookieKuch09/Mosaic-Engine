#include "../../include/systems/window.hpp"
#include "../../include/systems/contexts.hpp"

#include <GLFW/glfw3.h>

namespace Mosaic::Systems
{
    Window::Window(GlobalContext& context)
        : mGlobalContext(context), EventLayer(context.mEventManager, context.mEventManager)
    {
    }

    void Window::LoadSettings()
    {
        auto& settingsFile = mGlobalContext.GetGlobalSettings();

        mTitle = settingsFile.Get<std::string>("Window.Title");
        auto size = settingsFile.Get<std::uint32_t, 2>("Window.Size");

        mSize.x = size[0];
        mSize.y = size[1];
    }

    void Window::Start()
    {
        LoadSettings();

        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        mHandle = glfwCreateWindow(mSize.x, mSize.y, mTitle.c_str(), nullptr, nullptr);
    }

    void Window::Update()
    {
        glfwPollEvents();

        if (glfwWindowShouldClose(static_cast<GLFWwindow*>(mHandle)))
        {
            PostGlobalEvent<ApplicationQuitEvent>();
        }

        GetPosition();

        auto size = static_cast<glm::ivec2>(mSize);

        glfwGetWindowSize(static_cast<GLFWwindow*>(mHandle), &size.x, &size.y);

        if (static_cast<glm::uvec2>(size) != mSize)
        {
            mSize = static_cast<glm::uvec2>(size);

            PostGlobalEvent<WindowResizeEvent>(mSize);
        }

        auto pos = static_cast<glm::ivec2>(mPosition);

        glfwGetWindowPos(static_cast<GLFWwindow*>(mHandle), &pos.x, &pos.y);

        if (static_cast<glm::uvec2>(pos) != mPosition)
        {
            mPosition = static_cast<glm::uvec2>(pos);

            PostGlobalEvent<WindowMoveEvent>(mPosition);
        }
    }

    void Window::Stop()
    {
        glfwDestroyWindow(static_cast<GLFWwindow*>(mHandle));
        glfwTerminate();
    }

    void Window::SetPosition(const glm::uvec2& position)
    {
        mPosition = position;

        glfwSetWindowPos(static_cast<GLFWwindow*>(mHandle), position.x, position.y);
    }

    void Window::SetSize(const glm::uvec2& size)
    {
        mSize = size;

        glfwSetWindowSize(static_cast<GLFWwindow*>(mHandle), size.x, size.y);
    }

    void Window::SetTitle(const std::string& title)
    {
        mTitle = title;

        glfwSetWindowTitle(static_cast<GLFWwindow*>(mHandle), mTitle.c_str());
    }

    glm::uvec2 Window::GetPosition()
    {
        return mPosition;
    }

    glm::uvec2 Window::GetSize()
    {
        return mSize;
    }

    std::string Window::GetTitle()
    {
        return mTitle;
    }
}