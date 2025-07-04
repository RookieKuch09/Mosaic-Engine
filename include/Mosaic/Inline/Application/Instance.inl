#pragma once

#include <Mosaic/Application/Instance.hpp>

namespace Mosaic
{
    template <typename T> requires std::is_base_of_v<Application, T>
    Instance<T>::Instance()
        : mApplication(), mInstanceResources(mConsole, mWindow, mRenderer, mEntityManager, mEventManager), mEntityManager(mInstanceResources), mEventManager(mInstanceResources), mRunning(true), mWindow(mInstanceResources), mRenderer(mInstanceResources)
    {
    }

    template <typename T> requires std::is_base_of_v<Application, T>
    std::int32_t Instance<T>::Run()
    {
        try
        {
            mApplication.Setup(mInstanceResources);

            mEventManager.AddResponder(this, &Instance::OnApplicationExit);

            mRenderer.Setup();

            mWindow.Create();
            mRenderer.Create();

            while (mRunning)
            {
                mWindow.Update();
                mRenderer.Update();

                mEntityManager.Update();
                mEventManager.Update();
            }

            mRenderer.Destroy();
            mWindow.Destroy();

            return mExitCode;
        }
        catch (...)
        {
            // TODO: replace with log statement
        }
    }

    template <typename T> requires std::is_base_of_v<Application, T>
    void Instance<T>::OnApplicationExit(InstanceResources&, const ApplicationExitEvent& event)
    {
        mRunning = false;

        mExitCode = event.ExitCode;
    }
}