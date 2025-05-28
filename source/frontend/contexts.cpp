#include "../../include/frontend/contexts.hpp"

namespace Mosaic::Frontend
{
    LocalContext::LocalContext(LocalContextManager& localContextManager)
        : mLocalContextManager(localContextManager), mStarted(false)
    {
        mLocalContextManager.Register(this);
    }

    LocalContext::~LocalContext()
    {
        mLocalContextManager.Deregister(this);
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

    void LocalContextManager::Start()
    {
        FlushQueuedStartContexts();
        FlushQueuedStopContexts();

        for (const auto& context : mContexts)
        {
            auto& ctx = context.get();

            ctx.mStarted = true;
            ctx.Start();
        }
    }

    void LocalContextManager::Update()
    {
        FlushQueuedStartContexts();
        FlushQueuedStopContexts();

        for (const auto& context : mContexts)
        {
            auto& ctx = context.get();

            if (not ctx.mStarted)
            {
                ctx.mStarted = true;
                ctx.Start();
            }

            ctx.Update();
        }
    }

    void LocalContextManager::Stop()
    {
        for (const auto& context : mContexts)
        {
            auto& ctx = context.get();

            if (ctx.mStarted)
            {
                ctx.Stop();
            }
        }

        for (const auto& context : mContexts)
        {
            auto& ctx = context.get();

            if (ctx.mStarted)
            {
                ctx.Stop();
            }
        }
    }

    void LocalContextManager::Register(LocalContext* context)
    {
        mStartQueuedContexts.push_back(std::ref(*context));
    }

    void LocalContextManager::Deregister(LocalContext* context)
    {
        mStopQueuedContexts.push_back(std::ref(*context));
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
                    return &item.get() == &context.get();
                };

                auto it = std::find_if(vec.begin(), vec.end(), match);

                if (it != vec.end())
                {
                    vec.erase(it);
                }
                else
                {
                    Throw("{} is already registered and cannot be deregistered", contextType);
                }
            }

            queue.clear();
        };

        removeContext(mContexts, mStopQueuedContexts, "Local Context");
    }

    void GlobalContext::Start()
    {
        mWindow.Start();
        mRenderer.Start();

        mLocalContextManager.Start();
    }

    void GlobalContext::Update()
    {
        mWindow.Update();
        mRenderer.Update();

        mInputManager.Update();
        mEventManager.Update();

        mLocalContextManager.Update();
    }

    void GlobalContext::Stop()
    {
        mWindow.Stop();
        mRenderer.Stop();

        mLocalContextManager.Stop();
    }

    LocalContextManager& GlobalContext::GetContextManager()
    {
        return mLocalContextManager;
    }

    Registry& GlobalContext::GetRegistry()
    {
        return mResourceRegistry;
    }
}