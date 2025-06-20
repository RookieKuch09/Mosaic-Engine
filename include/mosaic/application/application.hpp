#pragma once

#include <mosaic/macros/exposure.hpp>

#include <type_traits>

namespace Mosaic
{
    struct Resources;

    class MOSAIC_PUBLIC_EXPOSURE Application
    {
    public:
        Application(const Application& other) = delete;
        Application(Application&& other) noexcept = default;

        Application& operator=(const Application& other) = delete;
        Application& operator=(Application&& other) noexcept = default;

        virtual ~Application() = default;

        virtual void Setup() = 0;

    protected:
        Application(Resources& resources);

        Resources& GetApplicationResources();

    private:
        Resources* mApplicationResources;

        template <typename T> requires std::is_base_of_v<Application, T>
        friend class Instance;
    };
}