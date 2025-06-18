#pragma once

#include <mosaic/application/instance.hpp>

namespace Mosaic
{
    template <typename T> requires std::is_base_of_v<Application, T>
    Instance<T>::Instance()
        : mInstance(mResources), mResources(mConsole, mECSManager, mEventManager), mECSManager(mResources), mEventManager(mResources), mRunning(true)
    {
    }

    template <typename T> requires std::is_base_of_v<Application, T>
    void Instance<T>::Setup()
    {
        mInstance.Setup();

        mEventManager.AddResponder(this, &Instance::OnApplicationExit);
    }

    template <typename T> requires std::is_base_of_v<Application, T>
    auto Instance<T>::Run() -> std::int32_t
    {
        while (mRunning)
        {
            mECSManager.Update();
            mEventManager.Update();
        }

        return mExitCode;
    }

    template <typename T> requires std::is_base_of_v<Application, T>
    void Instance<T>::OnApplicationExit(Resources&, const ApplicationExitEvent& event)
    {
        mRunning = false;

        mExitCode = event.ExitCode;
    }
}