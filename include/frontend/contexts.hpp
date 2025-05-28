#pragma once

#include <frontend/components.hpp>
#include <frontend/events.hpp>
#include <frontend/registry.hpp>

#include <frontend/rendering/graphs.hpp>
#include <frontend/rendering/targets.hpp>

#include <backend/inputs.hpp>
#include <backend/window.hpp>

#include <backend/rendering/renderer.hpp>

#include <functional>

namespace Mosaic::Frontend
{
    class LocalContextManager;

    class LogicContext
    {
    public:
        LogicContext(LocalContextManager& localContextManager);
        ~LogicContext();

        void Start();
        void Update();
        void Close();

    private:
        ComponentManager mComponentManager;
        EventManager mEventManager;

        LocalContextManager& mLocalContextManager;

        bool mStarted;

        friend class LocalContextManager;
    };

    class RenderContext
    {
    public:
        RenderContext(LocalContextManager& localContextManager);
        ~RenderContext();

        void Start();
        void Update();
        void Close();

    private:
        ComponentManager mComponentManager;
        EventManager mEventManager;

        Frontend::Rendering::RenderTarget mRenderTarget;
        Frontend::Rendering::LocalRenderGraph mLocalRenderGraph;

        LocalContextManager& mLocalContextManager;

        bool mStarted;

        friend class LocalContextManager;
    };

    class LocalContextManager
    {
    public:
        void Start();
        void Update();
        void Close();

        void Register(LogicContext* logicContext);
        void Register(RenderContext* renderContext);

        void Deregister(LogicContext* logicContext);
        void Deregister(RenderContext* renderContext);

    private:
        void FlushQueuedStartContexts();
        void FlushQueuedCloseContexts();

        std::vector<std::reference_wrapper<LogicContext>> mLogicContexts;
        std::vector<std::reference_wrapper<RenderContext>> mRenderContexts;

        std::vector<std::reference_wrapper<LogicContext>> mStartQueuedLogicContexts;
        std::vector<std::reference_wrapper<RenderContext>> mStartQueuedRenderContexts;

        std::vector<std::reference_wrapper<LogicContext>> mCloseQueuedLogicContexts;
        std::vector<std::reference_wrapper<RenderContext>> mCloseQueuedRenderContexts;
    };

    class GlobalContext
    {
    public:
        void Start();
        void Update();
        void Close();

    private:
        ComponentManager mComponentManager;
        EventManager mEventManager;
        Registry mResourceRegistry;
        LocalContextManager mLocalContextManager;

        Backend::Window mWindow;
        Backend::InputManager mInputManager;

        Backend::Rendering::Renderer mRenderer;

        Frontend::Rendering::GlobalRenderGraph mGlobalRenderGraph;
    };
}