#pragma once

#include <Mosaic/Application/Instance.hpp>

namespace Mosaic
{
    template <typename T> requires std::is_base_of_v<Application, T>
    Instance<T>::Instance()
        : mApplication(mInstanceResources), mInstanceResources(mConsole, mWindow, mRenderer, mECSManager, mEventManager), mECSManager(mInstanceResources), mEventManager(mInstanceResources), mRunning(true), mWindow(mInstanceResources), mRenderer(mInstanceResources)
    {
    }

    template <typename T> requires std::is_base_of_v<Application, T>
    void Instance<T>::Setup()
    {
        mApplication.Setup();

        mEventManager.AddResponder(this, &Instance::OnApplicationExit);
    }

    template <typename T> requires std::is_base_of_v<Application, T>
    std::int32_t Instance<T>::Run()
    {
        mRenderer.Setup();

        mWindow.Create();
        mRenderer.Create();

        while (mRunning)
        {
            mWindow.Update();
            mRenderer.Update();

            mECSManager.Update();
            mEventManager.Update();
        }

        mRenderer.Destroy();
        mWindow.Destroy();

        return mExitCode;
    }

    template <typename T> requires std::is_base_of_v<Application, T>
    void Instance<T>::OnApplicationExit(InstanceResources&, const ApplicationExitEvent& event)
    {
        mRunning = false;

        mExitCode = event.ExitCode;
    }
}