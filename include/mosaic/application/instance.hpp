#pragma once

#include <mosaic/api/exposure.hpp>

#include <mosaic/application/application.hpp>
#include <mosaic/application/resources.hpp>

#include <cstdint>
#include <type_traits>

namespace Mosaic
{
    template <typename T> requires std::is_base_of<Application, T>::value
    class MOSAIC_PUBLIC_EXPOSURE Instance
    {
    public:
        Instance()
            : mInstance(mResources)
        {
        }

        void Setup()
        {
            mInstance.Setup();
        }

        std::int32_t Run()
        {
            mResources.ECSManager.Update();

            return 0;
        }

    private:
        T mInstance;

        Resources mResources;
    };
}