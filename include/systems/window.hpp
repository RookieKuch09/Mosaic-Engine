#pragma once

#include "events.hpp"

#include <glm/glm.hpp>

#include <string>

namespace Mosaic::Systems
{
    enum class RendererAPI : int;

    class GlobalContext;

    struct WindowMoveEvent
    {
        glm::uvec2 Position;
    };

    struct WindowResizeEvent
    {
        glm::uvec2 Size;
    };

    class Window : public EventLayer
    {
    public:
        void SetPosition(const glm::uvec2& position);
        void SetSize(const glm::uvec2& size);
        void SetTitle(const std::string& title);

        glm::uvec2 GetPosition();
        glm::uvec2 GetSize();
        std::string GetTitle();

    private:
        Window(GlobalContext& context);

        void Start();
        void Update();
        void Stop();

        void LoadSettings();

        void* mHandle;
        GlobalContext& mGlobalContext;

        std::string mTitle;
        glm::uvec2 mSize;
        glm::uvec2 mPosition;

        friend class Implementation;
        friend class GlobalContext;
    };
}