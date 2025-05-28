#pragma once

#include "components.hpp"
#include "events.hpp"
#include "graphs.hpp"
#include "inputs.hpp"
#include "registry.hpp"
#include "renderer.hpp"
#include "window.hpp"

#include <functional>

namespace Mosaic::Frontend
{
    class LocalContextManager;

    class LocalContext
    {
    public:
        LocalContext(LocalContextManager& localContextManager);
        ~LocalContext();

        void Start();
        void Update();
        void Stop();

    private:
        ComponentManager mComponentManager;
        EventManager mEventManager;

        LocalContextManager& mLocalContextManager;

        bool mStarted;

        friend class LocalContextManager;
        friend class GlobalContext;
        friend class Component;
    };

    class LocalContextManager
    {
    public:
        void Start();
        void Update();
        void Stop();

        void Register(LocalContext* context);
        void Deregister(LocalContext* context);

    private:
        void FlushQueuedStartContexts();
        void FlushQueuedStopContexts();

        std::vector<std::reference_wrapper<LocalContext>> mContexts;
        std::vector<std::reference_wrapper<LocalContext>> mStartQueuedContexts;
        std::vector<std::reference_wrapper<LocalContext>> mStopQueuedContexts;
    };

    class GlobalContext
    {
    public:
        void Start();
        void Update();
        void Stop();

        template <typename T, typename... Args>
        T& NewResource(const std::string& id, Args&... constructorParams)
        {
            auto resource = std::make_shared<T>(constructorParams...);
            mResourceRegistry.Add<T>(id, std::move(resource));

            return mResourceRegistry.Get<T>(id);
        }

        LocalContextManager& GetContextManager();
        Registry& GetRegistry();

    private:
        EventManager mEventManager;
        Registry mResourceRegistry;
        LocalContextManager mLocalContextManager;
        Window mWindow;
        InputManager mInputManager;
        Renderer mRenderer;
        GlobalRenderGraph mGlobalRenderGraph;
    };
}