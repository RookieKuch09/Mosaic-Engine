#pragma once

#include <glm/glm.hpp>

#include <string>

namespace Mosaic
{
    enum class WindowVisibility : std::int32_t;

    struct WindowStateComponent
    {
        glm::uvec2 Size;
        glm::uvec2 Position;

        std::string Title;

        WindowVisibility Visibility;
    };
}