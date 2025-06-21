#pragma once

#include <mosaic/macros/exposure.hpp>

#include <glm/glm.hpp>

#include <string>

namespace Mosaic
{
    enum class WindowVisibility
    {
        Minimised,
        Windowed,
        Fullscreen,
        Borderless,
        Destroy,
    };

    using WindowTitle = std::string;
    using WindowSize = glm::uvec2;
    using WindowPosition = glm::uvec2;

    class MOSAIC_PUBLIC_EXPOSURE WindowResources
    {
    public:
        WindowSize Size;
        WindowPosition Position;
        WindowTitle Title;
        WindowVisibility Visibility;

    private:
        WindowResources();
        ~WindowResources() = default;

        friend class Window;
    };
}