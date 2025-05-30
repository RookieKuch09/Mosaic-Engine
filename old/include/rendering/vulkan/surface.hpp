#pragma once

#include <vulkan/vulkan.hpp>

namespace Mosaic::Systems::Internal::Windowing
{
    class Window;
}

namespace Mosaic::Systems::Internal::Rendering
{
    class VulkanInstance;
    class VulkanPhysicalDevice;

    class VulkanSurface
    {
    public:
        void SelectFormat(VulkanPhysicalDevice& physicalDevice, vk::Format format, vk::ColorSpaceKHR space);

        void Create(Windowing::Window& window, VulkanInstance& instance);

        vk::SurfaceKHR& GetHandle();
        vk::SurfaceFormatKHR& GetFormat();

    private:
        vk::UniqueSurfaceKHR mSurface;
        vk::SurfaceFormatKHR mFormat;
    };
}