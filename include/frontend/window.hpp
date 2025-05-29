#pragma once

#include "events.hpp"

#include <glm/glm.hpp>

#include <functional>
#include <string>
#include <unordered_map>

namespace Mosaic::Frontend
{
    class GlobalContext;

    struct WindowMoveEvent
    {
        glm::fvec2 Position;
    };

    struct WindowResizeEvent
    {
        glm::uvec2 Size;
    };

    class Window;

    class WindowBackend : public EventLayer
    {
    public:
        WindowBackend(GlobalContext& context, glm::uvec2& size, glm::uvec2& position, std::string& title);
        virtual ~WindowBackend() = default;

        virtual void Start() = 0;
        virtual void Update() = 0;
        virtual void Stop() = 0;

        virtual void SetPosition() = 0;
        virtual void SetSize() = 0;
        virtual void SetTitle() = 0;

        virtual void GetPosition() = 0;
        virtual void GetSize() = 0;
        virtual void GetTitle() = 0;

    protected:
        Window& mOwner;

        glm::uvec2& mSize;
        glm::uvec2& mPosition;
        std::string& mTitle;
    };

    class Window
    {
    public:
        Window(GlobalContext& context);

        void Start();
        void Update();
        void Stop();

        void SetPosition(const glm::uvec2& position);
        void SetSize(const glm::uvec2& size);
        void SetTitle(const std::string& title);

        glm::uvec2 GetPosition();
        glm::uvec2 GetSize();
        std::string GetTitle();

    private:
        WindowBackend* mWindowBackend;
        GlobalContext& mGlobalContext;

        std::string mTitle;
        glm::uvec2 mSize;
        glm::uvec2 mPosition;

        std::unordered_map<std::string, std::function<WindowBackend*()>> mBackendMapping;

        friend class WindowBackend;
    };
}