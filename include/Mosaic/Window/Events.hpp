#pragma once

#include <glm/glm.hpp>

#include <string>

namespace Mosaic
{
    enum class WindowVisibility : std::int32_t;

    struct WindowSizeChangeEvent
    {
        glm::uvec2 NewSize;
    };

    struct WindowPositionChangeEvent
    {
        glm::uvec2 NewPosition;
    };

    struct WindowVisibilityChangeEvent
    {
        WindowVisibility NewVisibility;
    };

    struct WindowTitleChangeEvent
    {
        std::string NewTitle;
    };

    struct WindowSizeChangeRequest
    {
        glm::uvec2 SizeRequest;
    };

    struct WindowPositionChangeRequest
    {
        glm::uvec2 PositionRequest;
    };

    struct WindowVisibilityChangeRequest
    {
        WindowVisibility VisibilityRequest;
    };

    struct WindowTitleChangeRequest
    {
        std::string TitleRequest;
    };
}