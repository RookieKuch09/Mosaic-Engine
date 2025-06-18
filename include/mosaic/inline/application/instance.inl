#pragma once

#include <mosaic/application/instance.hpp>

namespace Mosaic
{
    template <typename T> requires std::is_base_of_v<Application, T>
    Instance<T>::Instance()
        : mInstance(mResources)
    {
    }

    template <typename T> requires std::is_base_of_v<Application, T>
    void Instance<T>::Setup()
    {
        mInstance.Setup();
    }

    template <typename T> requires std::is_base_of_v<Application, T>
    auto Instance<T>::Run() -> std::int32_t
    {
        mResources.ECSManager.Update();

        return 0;
    }
}