#pragma once

#include <string>
#include <unordered_set>

#include <vulkan/vulkan.hpp>

namespace Mosaic::Systems::Internal::Windowing
{
    class Window;
}

namespace Mosaic::Systems::Internal::Rendering
{
    class VulkanInstance
    {
    public:
        void SelectWindowExtensions(const Windowing::Window& window);

        void SelectExtensions(const std::unordered_set<std::string>& whitelist, const std::unordered_set<std::string>& blacklist);

        void SelectLayers(const std::unordered_set<std::string>& selected);

        void Create();

        vk::Instance& Get();

    private:
        vk::UniqueInstance mInstance;

        std::vector<std::string> mExtensions;
        std::vector<std::string> mLayers;
    };
}