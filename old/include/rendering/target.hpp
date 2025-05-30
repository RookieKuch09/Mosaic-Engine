#pragma once

#include "rendering/commands.hpp"
#include "utilities/rect.hpp"

namespace Mosaic::Systems::Internal::Windowing
{
    class Window;
}

namespace Mosaic::Systems::Internal::Rendering
{
    class RendererCommandManager;
}

namespace Mosaic::Systems::Internal::Rendering
{
    class RenderTarget
    {
    public:
        RenderTarget(Types::Rectangle&& area, RendererCommandManager& renderer);

    private:
        void Render();

        Types::Rectangle mArea;

        RendererCommandManager& mRendererCommandManager;
    };
}