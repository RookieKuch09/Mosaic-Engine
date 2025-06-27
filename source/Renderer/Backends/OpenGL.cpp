#include <Mosaic/Rendering/Backends/OpenGL.hpp>

#include <Mosaic/Application/Resources.hpp>

#include <Mosaic/Debug/Console.hpp>

#include <Mosaic/Window/Backend.hpp>
#include <Mosaic/Window/Window.hpp>

#include <Mosaic/Rendering/Renderer.hpp>

#include <GL/glew.h>

namespace Mosaic
{
    OpenGLRendererBackend::OpenGLRendererBackend(InstanceResources& instanceResources, glm::uvec2 size, glm::fvec4 clearColour, RendererVSyncMode vsyncMode)
        : RendererBackend(instanceResources, size, clearColour, vsyncMode)
    {
    }

    void OpenGLRendererBackend::Setup()
    {
        auto& window = mInstanceResources.Window.mBackend;

        // TODO: allow for user to select OpenGL version
        // TODO: ensure version is supported by system
        window->GL_CreateContext({3, 3});
    }

    void OpenGLRendererBackend::Create()
    {
        auto& window = mInstanceResources.Window.mBackend;
        auto& console = mInstanceResources.Console;

        std::uint32_t swapInterval = GetSwapInterval();

        window->GL_ActivateContext();
        window->GL_SetSwapInterval(swapInterval);

        if (glewInit() != GLEW_OK)
        {
            console.Halt(1, "Failed to load OpenGL extensions from drivers");
        }

        auto& r = mClearColour.x;
        auto& g = mClearColour.y;
        auto& b = mClearColour.z;
        auto& a = mClearColour.w;

        glClearColor(r, g, b, a);

        glViewport(0, 0, mSize.x, mSize.y);
    }

    void OpenGLRendererBackend::Update()
    {
        auto& window = mInstanceResources.Window.mBackend;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // TODO: add draw calls here

        window->GL_SwapBuffers();
    }

    void OpenGLRendererBackend::Destroy()
    {
    }

    void OpenGLRendererBackend::SetClearColour(glm::fvec4 clearColour)
    {
        mClearColour = clearColour;

        auto& r = mClearColour.x;
        auto& g = mClearColour.y;
        auto& b = mClearColour.z;
        auto& a = mClearColour.w;

        glClearColor(r, g, b, a);
    }

    void OpenGLRendererBackend::SetVSyncMode(RendererVSyncMode vsyncMode)
    {
        mVSyncMode = vsyncMode;

        auto& window = mInstanceResources.Window.mBackend;

        std::uint32_t swapInterval = GetSwapInterval();

        window->GL_SetSwapInterval(swapInterval);
    }

    glm::fvec4 OpenGLRendererBackend::GetClearColour()
    {
        return mClearColour;
    }

    RendererVSyncMode OpenGLRendererBackend::GetVSyncMode()
    {
        return mVSyncMode;
    }

    void OpenGLRendererBackend::Resize(glm::uvec2 size)
    {
        mSize = size;

        glViewport(0, 0, mSize.x, mSize.y);
    }

    std::uint32_t OpenGLRendererBackend::GetSwapInterval()
    {
        switch (mVSyncMode)
        {
            case RendererVSyncMode::Disabled:
            {
                return 0;
            }
            case RendererVSyncMode::Strict:
            {
                return 1;
            }
            case RendererVSyncMode::Relaxed:
            {
                // TODO: add safety checks for if -1 is supported as a swap interval
                return -1;
            }
        }
    }
}