#include "../../../include/backend/windows/glfw.hpp"

#include <GLFW/glfw3.h>

namespace Mosaic::Backend::Windows
{
    GLFWWindow::GLFWWindow(Frontend::GlobalContext& context, glm::uvec2& size, glm::uvec2& position, std::string& title)
        : Frontend::WindowBackend(context, size, position, title)
    {
    }

    void GLFWWindow::Start()
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        mHandle = glfwCreateWindow(mSize.x, mSize.y, mTitle.c_str(), nullptr, nullptr);
    }

    void GLFWWindow::Update()
    {
        glfwPollEvents();

        GetPosition();
        GetSize();

        if (glfwWindowShouldClose(mHandle))
        {
            EmitGlobalEvent<Frontend::ApplicationQuitEvent>({});
        }
    }

    void GLFWWindow::Stop()
    {
        glfwDestroyWindow(mHandle);
        glfwTerminate();
    }

    void GLFWWindow::SetPosition()
    {
        glfwSetWindowPos(mHandle, mPosition.x, mPosition.y);
    }

    void GLFWWindow::SetSize()
    {
        glfwSetWindowSize(mHandle, mSize.x, mSize.y);
    }

    void GLFWWindow::SetTitle()
    {
        glfwSetWindowTitle(mHandle, mTitle.c_str());
    }

    void GLFWWindow::GetPosition()
    {
        int x;
        int y;

        glfwGetWindowPos(mHandle, &x, &y);

        if (x != mPosition.x or y != mPosition.y)
        {
            mPosition.x = x;
            mPosition.y = y;

            EmitGlobalEvent<Frontend::WindowMoveEvent>({mPosition});
        }
    }

    void GLFWWindow::GetSize()
    {
        int x;
        int y;

        glfwGetWindowSize(mHandle, &x, &y);

        if (x != mSize.x or y != mSize.y)
        {
            mSize.x = x;
            mSize.y = y;

            EmitGlobalEvent<Frontend::WindowResizeEvent>({mSize});
        }
    }

    void GLFWWindow::GetTitle()
    {
        mTitle = glfwGetWindowTitle(mHandle);
    }
}