#pragma once

#include <Mosaic/Application/Instance.hpp>

namespace Mosaic
{
    template <typename T> requires std::is_base_of_v<Application, T>
    Instance<T>::Instance()
        : mInstance(mInstanceResources), mInstanceResources(mConsole, mWindow, mECSManager, mEventManager), mECSManager(mInstanceResources), mEventManager(mInstanceResources), mRunning(true), mWindow(mInstanceResources)
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
    void Instance<T>::OnApplicationExit(InstanceResources&, const ApplicationExitEvent& event)
    {
        mRunning = false;

        mExitCode = event.ExitCode;
    }
}