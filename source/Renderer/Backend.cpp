#include <Mosaic/Renderer/Backend.hpp>

namespace Mosaic
{
    RendererBackend::RendererBackend(InstanceResources& instanceResources, glm::uvec2 size, glm::fvec4 clearColour, RendererVSyncMode vsyncMode)
        : mSize(size), mClearColour(clearColour), mVSyncMode(vsyncMode), mInstanceResources(instanceResources)
    {
    }
}