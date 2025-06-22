#include <Mosaic/Window/Backend.hpp>

namespace Mosaic
{
    WindowBackend::WindowBackend(InstanceResources& instanceResources, glm::uvec2 size, glm::uvec2 position, const std::string& title, WindowVisibility visibility)
        : mSize(size), mPosition(position), mTitle(title), mVisibility(visibility), mInstanceResources(instanceResources)
    {
    }
}