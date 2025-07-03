// TODO: add conditional compilation based on CMake macro definitions

#include <Mosaic/Window/Backends/GLFW.hpp>
#include <Mosaic/Window/Window.hpp>

#include <Mosaic/Application/Resources.hpp>

#include <Mosaic/Debug/Console/Resource.hpp>

#include <GLFW/glfw3.h>

namespace Mosaic
{
    void GLFWWindowBackend::GL_CreateContext(glm::uvec2 version)
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version.x);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version.y);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    void GLFWWindowBackend::GL_ActivateContext()
    {
        glfwMakeContextCurrent(static_cast<GLFWwindow*>(mHandle));
    }

    void GLFWWindowBackend::GL_SetSwapInterval(std::uint32_t interval)
    {
        glfwSwapInterval(interval);
    }

    void GLFWWindowBackend::GL_SwapBuffers()
    {
        glfwSwapBuffers(static_cast<GLFWwindow*>(mHandle));
    }

    GLFWWindowBackend::GLFWWindowBackend(InstanceResources& instanceResources, glm::uvec2 size, glm::uvec2 position, const std::string& title, WindowVisibility visibility)
        : WindowBackend(instanceResources, size, position, title, visibility)
    {
        if (glfwInit() != GLFW_TRUE)
        {
            // TODO: replace with error management
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }

    void GLFWWindowBackend::Create()
    {
        mHandle = glfwCreateWindow(mSize.x, mSize.y, mTitle.c_str(), nullptr, nullptr);

        if (!mHandle)
        {
            // TODO: replace with error management
        }
    }

    void GLFWWindowBackend::Update()
    {
        glfwPollEvents();
    }

    void GLFWWindowBackend::Destroy()
    {
        if (mHandle)
        {
            glfwDestroyWindow(static_cast<GLFWwindow*>(mHandle));

            mHandle = nullptr;
        }

        glfwTerminate();
    }

    void GLFWWindowBackend::SetSize(glm::uvec2 size)
    {
        glfwSetWindowSize(static_cast<GLFWwindow*>(mHandle), size.x, size.y);

        mSize = size;
    }

    void GLFWWindowBackend::SetPosition(glm::uvec2 position)
    {
        glfwSetWindowPos(static_cast<GLFWwindow*>(mHandle), position.x, position.y);

        mPosition = position;
    }

    void GLFWWindowBackend::SetTitle(const std::string& title)
    {
        glfwSetWindowTitle(static_cast<GLFWwindow*>(mHandle), title.c_str());

        mTitle = title;
    }

    void GLFWWindowBackend::SetVisibility(WindowVisibility visibility)
    {
        switch (visibility)
        {
            case WindowVisibility::Minimised:
            {
                glfwIconifyWindow(static_cast<GLFWwindow*>(mHandle));

                break;
            }
            case WindowVisibility::Windowed:
            {
                if (glfwGetWindowMonitor(static_cast<GLFWwindow*>(mHandle)))
                {
                    const auto& size = mWindowedSize;
                    const auto& pos = mWindowedPosition;

                    glfwSetWindowMonitor(static_cast<GLFWwindow*>(mHandle), nullptr, pos.x, pos.y, size.x, size.y, 0);
                    glfwSetWindowAttrib(static_cast<GLFWwindow*>(mHandle), GLFW_DECORATED, GLFW_TRUE);
                }

                break;
            }

            case WindowVisibility::Fullscreen:
            {
                if (!glfwGetWindowMonitor(static_cast<GLFWwindow*>(mHandle)))
                {
                    int xpos, ypos, width, height;

                    glfwGetWindowPos(static_cast<GLFWwindow*>(mHandle), &xpos, &ypos);
                    glfwGetWindowSize(static_cast<GLFWwindow*>(mHandle), &width, &height);

                    mWindowedPosition = {xpos, ypos};
                    mWindowedSize = {width, height};
                }

                GLFWmonitor* monitor = glfwGetPrimaryMonitor();
                const GLFWvidmode* mode = glfwGetVideoMode(monitor);

                if (monitor && mode)
                {
                    glfwSetWindowMonitor(static_cast<GLFWwindow*>(mHandle), monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
                }

                break;
            }

            case WindowVisibility::Borderless:
            {
                if (!glfwGetWindowMonitor(static_cast<GLFWwindow*>(mHandle)))
                {
                    int xpos, ypos, width, height;

                    glfwGetWindowPos(static_cast<GLFWwindow*>(mHandle), &xpos, &ypos);
                    glfwGetWindowSize(static_cast<GLFWwindow*>(mHandle), &width, &height);

                    mWindowedPosition = {xpos, ypos};
                    mWindowedSize = {width, height};
                }

                GLFWmonitor* monitor = glfwGetPrimaryMonitor();
                const GLFWvidmode* mode = glfwGetVideoMode(monitor);

                if (monitor && mode)
                {
                    glfwSetWindowMonitor(static_cast<GLFWwindow*>(mHandle), nullptr, 0, 0, mode->width, mode->height, mode->refreshRate);
                    glfwSetWindowAttrib(static_cast<GLFWwindow*>(mHandle), GLFW_DECORATED, GLFW_FALSE);
                }

                break;
            }

            case WindowVisibility::Destroy:
            {
                glfwSetWindowShouldClose(static_cast<GLFWwindow*>(mHandle), GLFW_TRUE);

                break;
            }
        }
    }

    glm::uvec2 GLFWWindowBackend::GetSize()
    {
        glm::ivec2 size;

        glfwGetWindowSize(static_cast<GLFWwindow*>(mHandle), &size.x, &size.y);

        mSize = size;

        return mSize;
    }

    glm::uvec2 GLFWWindowBackend::GetPosition()
    {
        glm::ivec2 position;

        glfwGetWindowPos(static_cast<GLFWwindow*>(mHandle), &position.x, &position.y);

        mPosition = position;

        return mPosition;
    }

    std::string GLFWWindowBackend::GetTitle()
    {
        return mTitle;
    }

    WindowVisibility GLFWWindowBackend::GetVisibility()
    {
        if (glfwWindowShouldClose(static_cast<GLFWwindow*>(mHandle)))
        {
            mVisibility = WindowVisibility::Destroy;
        }
        else if (glfwGetWindowAttrib(static_cast<GLFWwindow*>(mHandle), GLFW_ICONIFIED))
        {
            mVisibility = WindowVisibility::Minimised;
        }
        else if (glfwGetWindowMonitor(static_cast<GLFWwindow*>(mHandle)))
        {
            mVisibility = WindowVisibility::Fullscreen;
        }
        else if (!glfwGetWindowAttrib(static_cast<GLFWwindow*>(mHandle), GLFW_DECORATED))
        {
            mVisibility = WindowVisibility::Borderless;
        }
        else
        {
            mVisibility = WindowVisibility::Windowed;
        }

        return mVisibility;
    }
}