#include "../../include/frontend/contexts.hpp"

namespace Mosaic::Frontend
{
    LocalContext::LocalContext(GlobalContext& globalContext)
        : mGlobalContext(globalContext), mStarted(false)
    {
        mGlobalContext.mLocalContextManager.Register(this);
    }

    LocalContext::~LocalContext()
    {
        mGlobalContext.mLocalContextManager.Deregister(this);
        mGlobalContext.mResourceRegistry.Remove(*this);

        Utilities::LogNotice("Deleting Context");
    }

    void LocalContext::Start()
    {
        mComponentManager.Start();
    }

    void LocalContext::Update()
    {
        mComponentManager.Update();
        mEventManager.Update();
    }

    void LocalContext::Stop()
    {
        mComponentManager.Stop();
    }

    void LocalContext::RemoveFromRegistry()
    {
        Utilities::LogNotice("Deleting Context");
        mComponentManager.Cleanup();
        mGlobalContext.mResourceRegistry.Remove(*this);
    }

    void LocalContextManager::Start()
    {
        FlushQueuedStartContexts();
        FlushQueuedStopContexts();

        for (const auto& context : mContexts)
        {
            context->mStarted = true;
            context->Start();
        }
    }

    void LocalContextManager::Update()
    {
        FlushQueuedStartContexts();
        FlushQueuedStopContexts();

        for (const auto& context : mContexts)
        {
            if (not context->mStarted)
            {
                context->mStarted = true;
                context->Start();
            }

            context->Update();
        }
    }

    void LocalContextManager::Stop()
    {
        for (const auto& context : mContexts)
        {
            if (context->mStarted)
            {
                context->Stop();
            }
        }

        for (const auto& context : mContexts)
        {
            if (context->mStarted)
            {
                context->Stop();
            }
        }
    }

    void LocalContextManager::Register(LocalContext* context)
    {
        mStartQueuedContexts.push_back(context);
    }

    void LocalContextManager::Deregister(LocalContext* context)
    {
        mStopQueuedContexts.push_back(context);
    }

    void LocalContextManager::Cleanup()
    {
        for (LocalContext* context : mContexts)
        {
            if (context)
            {
                context->RemoveFromRegistry();
            }
        }

        for (LocalContext* context : mStartQueuedContexts)
        {
            if (context)
            {
                context->RemoveFromRegistry();
            }
        }

        for (LocalContext* context : mStopQueuedContexts)
        {
            if (context)
            {
                context->RemoveFromRegistry();
            }
        }
    }

    void LocalContextManager::FlushQueuedStartContexts()
    {
        std::move(mStartQueuedContexts.begin(), mStartQueuedContexts.end(), std::back_inserter(mContexts));

        mStartQueuedContexts.clear();
    }

    void LocalContextManager::FlushQueuedStopContexts()
    {
        auto removeContext = [](auto& vec, auto& queue, const char* contextType)
        {
            for (const auto& context : queue)
            {
                auto match = [&](const auto& item)
                {
                    return item == context;
                };

                auto it = std::find_if(vec.begin(), vec.end(), match);

                if (it != vec.end())
                {
                    vec.erase(it);
                }
                else
                {
                    Utilities::Throw("{} is already registered and cannot be deregistered", contextType);
                }
            }

            queue.clear();
        };

        removeContext(mContexts, mStopQueuedContexts, "Local Context");
    }

    GlobalContext::GlobalContext(const std::string& configPath)
        : EventLayer(mEventManager, mEventManager), mRunning(true), mSettingsFile(configPath), mWindow(*this)
    {
    }

    void GlobalContext::Start()
    {
        mSettingsFile.Open();

        mWindow.Start();
        mRenderer.Start();

        mLocalContextManager.Start();
    }

    void GlobalContext::Update()
    {
        SetGlobalCallback(this, &GlobalContext::OnAppExit);

        while (mRunning)
        {
            mLocalContextManager.Update();

            mInputManager.Update();

            mWindow.Update();

            mEventManager.Update();

            mRenderer.Update();
        }
    }

    void GlobalContext::Stop()
    {
        mWindow.Stop();
        mRenderer.Stop();

        mLocalContextManager.Stop();
        mLocalContextManager.Cleanup();
    }

    Utilities::TOMLFile& GlobalContext::GetGlobalSettings()
    {
        return mSettingsFile;
    }

    void GlobalContext::OnAppExit(const ApplicationQuitEvent& event)
    {
        mRunning = false;
    }
}