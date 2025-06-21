// TODO: add conditional compilation based on CMake macro definitions

#include <mosaic/application/resources.hpp>
#include <mosaic/debug/console.hpp>

#include <mosaic/window/resources.hpp>
#include <mosaic/window/window.hpp>

#include <GLFW/glfw3.h>

namespace Mosaic
{
    struct Window::Backend::Storage
    {
        GLFWwindow* Handle;

        glm::ivec2 WindowedPosition;
        glm::ivec2 WindowedSize;
    };

    Window::Backend::Backend(WindowResources& windowResources, ApplicationResources& applicationResources)
        : mWindowResources(windowResources), mApplicationResources(applicationResources)
    {
        if (glfwInit() != GLFW_TRUE)
        {
            applicationResources.Console.Halt(1, "Failed to initialise window backend");
        }

        mStorage = new Storage();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }

    Window::Backend::~Backend()
    {
    }

    void Window::Backend::Create()
    {
        auto& size = mWindowResources.Size;
        auto& title = mWindowResources.Title;

        mStorage->Handle = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);

        if (not mStorage->Handle)
        {
            mApplicationResources.Console.Halt(1, "Failed to create window");
        }
    }

    void Window::Backend::Destroy()
    {
        if (mStorage)
        {
            if (mStorage->Handle)
            {
                glfwDestroyWindow(mStorage->Handle);

                mStorage->Handle = nullptr;
            }

            delete mStorage;

            mStorage = nullptr;

            glfwTerminate();
        }
    }

    void Window::Backend::GetState()
    {
        glm::ivec2 position;
        glm::ivec2 size;

        glfwGetWindowSize(mStorage->Handle, &size.x, &size.y);
        glfwGetWindowPos(mStorage->Handle, &position.x, &position.y);

        mWindowResources.Position = position;
        mWindowResources.Size = size;

        if (glfwWindowShouldClose(mStorage->Handle))
        {
            mWindowResources.Visibility = WindowVisibility::Destroy;

            return;
        }

        if (glfwGetWindowAttrib(mStorage->Handle, GLFW_ICONIFIED))
        {
            mWindowResources.Visibility = WindowVisibility::Minimised;
            return;
        }

        if (glfwGetWindowMonitor(mStorage->Handle))
        {
            mWindowResources.Visibility = WindowVisibility::Fullscreen;

            return;
        }

        if (not glfwGetWindowAttrib(mStorage->Handle, GLFW_DECORATED))
        {
            mWindowResources.Visibility = WindowVisibility::Borderless;
        }
        else
        {
            mWindowResources.Visibility = WindowVisibility::Windowed;
        }
    }

    void Window::Backend::SetState()
    {
        SetSize(mWindowResources.Size);
        SetPosition(mWindowResources.Position);
        SetTitle(mWindowResources.Title);
        SetVisibility(mWindowResources.Visibility);
    }

    void Window::Backend::SetSize(WindowSize size)
    {
        glfwSetWindowSize(mStorage->Handle, size.x, size.y);
    }

    void Window::Backend::SetPosition(WindowPosition position)
    {
        glfwSetWindowPos(mStorage->Handle, position.x, position.y);
    }

    void Window::Backend::SetTitle(WindowTitle title)
    {
        glfwSetWindowTitle(mStorage->Handle, title.c_str());
    }

    void Window::Backend::SetVisibility(WindowVisibility visibility)
    {
        GLFWwindow* handle = mStorage->Handle;

        switch (visibility)
        {
            case WindowVisibility::Minimised:
            {
                glfwIconifyWindow(handle);

                break;
            }
            case WindowVisibility::Windowed:
            {
                if (glfwGetWindowMonitor(handle))
                {
                    const auto& size = mStorage->WindowedSize;
                    const auto& pos = mStorage->WindowedPosition;

                    glfwSetWindowMonitor(handle, nullptr, pos.x, pos.y, size.x, size.y, 0);
                    glfwSetWindowAttrib(handle, GLFW_DECORATED, GLFW_TRUE);
                }

                break;
            }

            case WindowVisibility::Fullscreen:
            {
                if (not glfwGetWindowMonitor(handle))
                {
                    int xpos, ypos, width, height;

                    glfwGetWindowPos(handle, &xpos, &ypos);
                    glfwGetWindowSize(handle, &width, &height);

                    mStorage->WindowedPosition = {xpos, ypos};
                    mStorage->WindowedSize = {width, height};
                }

                GLFWmonitor* monitor = glfwGetPrimaryMonitor();
                const GLFWvidmode* mode = glfwGetVideoMode(monitor);

                if (monitor and mode)
                {
                    glfwSetWindowMonitor(handle, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
                }

                break;
            }

            case WindowVisibility::Borderless:
            {
                if (not glfwGetWindowMonitor(handle))
                {
                    int xpos, ypos, width, height;

                    glfwGetWindowPos(handle, &xpos, &ypos);
                    glfwGetWindowSize(handle, &width, &height);

                    mStorage->WindowedPosition = {xpos, ypos};
                    mStorage->WindowedSize = {width, height};
                }

                GLFWmonitor* monitor = glfwGetPrimaryMonitor();
                const GLFWvidmode* mode = glfwGetVideoMode(monitor);

                if (monitor and mode)
                {
                    glfwSetWindowMonitor(handle, nullptr, 0, 0, mode->width, mode->height, mode->refreshRate);
                    glfwSetWindowAttrib(handle, GLFW_DECORATED, GLFW_FALSE);
                }

                break;
            }

            case WindowVisibility::Destroy:
            {
                glfwSetWindowShouldClose(handle, GLFW_TRUE);

                break;
            }
        }
    }

    void Window::Backend::Update()
    {
        glfwPollEvents();
    }
}