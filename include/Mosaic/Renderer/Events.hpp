#pragma once

#include <glm/glm.hpp>

namespace Mosaic
{
    enum class RendererVSyncMode : std::int32_t;

    struct RendererClearColourChangeRequest
    {
        glm::fvec4 ClearColourRequest;
    };

    struct RendererVSyncModeChangeRequest
    {
        RendererVSyncMode VSyncModeRequest;
    };
}