#pragma once

#include <Mosaic/Application/Instance.hpp>

namespace Mosaic
{
    template <typename T> requires std::is_base_of_v<Application, T>
    Instance<T>::Instance()
        : mApplication(), mInstanceResources(mConsole, mWindow, mRenderer, mECSManager, mEventManager), mECSManager(mInstanceResources), mEventManager(mInstanceResources), mRunning(true), mWindow(mInstanceResources), mRenderer(mInstanceResources)
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

                mECSManager.Update();
                mEventManager.Update();
            }

            mRenderer.Destroy();
            mWindow.Destroy();

            return mExitCode;
        }
        catch (const EarlyExit& earlyExit)
        {
            mConsole.Log<LogSeverity::Notice>("Early application exit: {} (exit code: {})", earlyExit.ExitMessage, earlyExit.ExitCode);

            return earlyExit.ExitCode;
        }
    }

    template <typename T> requires std::is_base_of_v<Application, T>
    void Instance<T>::OnApplicationExit(InstanceResources&, const ApplicationExitEvent& event)
    {
        mRunning = false;

        mExitCode = event.ExitCode;
    }
}