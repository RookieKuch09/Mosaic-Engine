#pragma once

#include <mosaic/api/exposure.hpp>

#include <type_traits>

namespace Mosaic
{
    struct Resources;

    class MOSAIC_PUBLIC_EXPOSURE Application
    {
    public:
        Application(const Application& other) = delete;
        Application(Application&& other) noexcept = default;

        auto operator=(const Application& other) -> Application& = delete;
        auto operator=(Application&& other) noexcept -> Application& = default;

        virtual ~Application() = default;

        virtual void Setup() = 0;

    protected:
        Application(Resources& resources);

        auto GetApplicationResources() -> Resources&;

    private:
        Resources* mApplicationResources;

        template <typename T> requires std::is_base_of_v<Application, T>
        friend class Instance;
    };
}