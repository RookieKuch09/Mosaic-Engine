#pragma once

#include <Mosaic/Macros/Exposure.hpp>

#include <cstdint>
#include <type_traits>

namespace Mosaic
{
    struct ApplicationExitEvent
    {
        std::int32_t ExitCode;
    };

    class InstanceResources;

    class MOSAIC_PUBLIC_EXPOSURE Application
    {
    public:
        Application(const Application& other) = delete;
        Application(Application&& other) noexcept = default;

        Application& operator=(const Application& other) = delete;
        Application& operator=(Application&& other) noexcept = default;

        virtual void Setup(InstanceResources& instanceResources) = 0;

    protected:
        Application() = default;
        virtual ~Application() = default;

    private:
        template <typename T> requires std::is_base_of_v<Application, T>
        friend class Instance;
    };
}