#include <frontend/contexts.hpp>

namespace Mosaic::Frontend
{
    void LocalContextManager::Start()
    {
        FlushQueuedStartContexts();
        FlushQueuedCloseContexts();

        for (const auto& context : mLogicContexts)
        {
            auto& ctx = context.get();

            ctx.mStarted = true;
            ctx.Start();
        }

        for (const auto& context : mRenderContexts)
        {
            auto& ctx = context.get();

            ctx.mStarted = true;
            ctx.Start();
        }
    }

    void LocalContextManager::Update()
    {
        FlushQueuedStartContexts();
        FlushQueuedCloseContexts();

        for (const auto& context : mLogicContexts)
        {
            auto& ctx = context.get();

            if (not ctx.mStarted)
            {
                ctx.mStarted = true;
                ctx.Start();
            }

            ctx.Update();
        }

        for (const auto& context : mRenderContexts)
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

    void LocalContextManager::Close()
    {
        for (const auto& context : mLogicContexts)
        {
            auto& ctx = context.get();

            if (ctx.mStarted)
            {
                ctx.Close();
            }
        }

        for (const auto& context : mRenderContexts)
        {
            auto& ctx = context.get();

            if (ctx.mStarted)
            {
                ctx.Close();
            }
        }
    }

    void LocalContextManager::Register(LogicContext* logicContext)
    {
        mStartQueuedLogicContexts.push_back(std::ref(*logicContext));
    }

    void LocalContextManager::Register(RenderContext* renderContext)
    {
        mStartQueuedRenderContexts.push_back(std::ref(*renderContext));
    }

    void LocalContextManager::Deregister(LogicContext* logicContext)
    {
        mCloseQueuedLogicContexts.push_back(std::ref(*logicContext));
    }

    void LocalContextManager::Deregister(RenderContext* renderContext)
    {
        mCloseQueuedRenderContexts.push_back(std::ref(*renderContext));
    }

    void LocalContextManager::FlushQueuedStartContexts()
    {
        std::move(mStartQueuedLogicContexts.begin(), mStartQueuedLogicContexts.end(), std::back_inserter(mLogicContexts));
        std::move(mStartQueuedRenderContexts.begin(), mStartQueuedRenderContexts.end(), std::back_inserter(mRenderContexts));

        mStartQueuedLogicContexts.clear();
        mStartQueuedRenderContexts.clear();
    }

    void LocalContextManager::FlushQueuedCloseContexts()
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
                    Throw("{} is not already registered and cannot be deregistered", contextType);
                }
            }

            queue.clear();
        };

        removeContext(mLogicContexts, mCloseQueuedLogicContexts, "Logic Context");
        removeContext(mRenderContexts, mCloseQueuedRenderContexts, "Render Context");
    }

    void GlobalContext::Start()
    {
        mWindow.Start();
        mRenderer.Start();

        mEventManager.Update();

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

    void GlobalContext::Close()
    {
        mWindow.Close();
        mRenderer.Close();

        mLocalContextManager.Close();
    }

    LogicContext::LogicContext(LocalContextManager& localContextManager)
        : mLocalContextManager(localContextManager), mStarted(false)
    {
        mLocalContextManager.Register(this);
    }

    LogicContext::~LogicContext()
    {
        mLocalContextManager.Deregister(this);
    }

    void LogicContext::Start()
    {
        mComponentManager.Start();
        mEventManager.Update();
    }

    void LogicContext::Update()
    {
        mComponentManager.Update();
        mEventManager.Update();
    }

    void LogicContext::Close()
    {
        mComponentManager.Close();
    }

    RenderContext::RenderContext(LocalContextManager& localContextManager)
        : mLocalContextManager(localContextManager), mStarted(false)
    {
        mLocalContextManager.Register(this);
    }

    RenderContext::~RenderContext()
    {
        mLocalContextManager.Deregister(this);
    }

    void RenderContext::Start()
    {
        mComponentManager.Start();
        mEventManager.Update();
    }

    void RenderContext::Update()
    {
        mComponentManager.Update();
        mEventManager.Update();
    }

    void RenderContext::Close()
    {
        mComponentManager.Close();
    }
}