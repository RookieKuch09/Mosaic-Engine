#pragma once

#include <mosaic/api/exposure.hpp>

#include <mosaic/application/application.hpp>
#include <mosaic/application/resources.hpp>

#include <cstdint>
#include <type_traits>

namespace Mosaic
{
    template <typename T> requires std::is_base_of_v<Application, T>
    class MOSAIC_PUBLIC_EXPOSURE Instance
    {
    public:
        Instance();

        void Setup();

        auto Run() -> std::int32_t;

    private:
        T mInstance;

        Resources mResources;
    };
}

#include <mosaic/inline/application/instance.inl>