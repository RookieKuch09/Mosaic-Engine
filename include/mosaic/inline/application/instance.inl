#pragma once

#include <mosaic/application/instance.hpp>

namespace Mosaic
{
    template <typename T> requires std::is_base_of_v<Application, T>
    Instance<T>::Instance()
        : mInstance(mApplicationResources), mApplicationResources(mConsole, mWindow, mECSManager, mEventManager), mECSManager(mApplicationResources), mEventManager(mApplicationResources), mRunning(true), mWindow(mApplicationResources)
    {
    }

    template <typename T> requires std::is_base_of_v<Application, T>
    void Instance<T>::Setup()
    {
        mInstance.Setup();

        mEventManager.AddResponder(this, &Instance::OnApplicationExit);
    }

    template <typename T> requires std::is_base_of_v<Application, T>
    std::int32_t Instance<T>::Run()
    {
        while (mRunning)
        {
            mWindow.Update();

            mECSManager.Update();
            mEventManager.Update();
        }

        return mExitCode;
    }

    template <typename T> requires std::is_base_of_v<Application, T>
    void Instance<T>::OnApplicationExit(ApplicationResources&, const ApplicationExitEvent& event)
    {
        mRunning = false;

        mExitCode = event.ExitCode;
    }
}