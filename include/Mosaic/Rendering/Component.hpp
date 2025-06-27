#pragma once

#include <glm/glm.hpp>

namespace Mosaic
{
    enum class RendererVSyncMode : std::int32_t;

    struct RendererStateComponent
    {
        glm::fvec4 ClearColour;
        RendererVSyncMode VSyncMode;
    };
}