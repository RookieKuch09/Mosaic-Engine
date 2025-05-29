#pragma once

#include "../utilities/toml.hpp"

#include "components.hpp"
#include "graphs.hpp"
#include "inputs.hpp"
#include "registry.hpp"
#include "renderer.hpp"
#include "window.hpp"

#include <functional>

namespace Mosaic::Frontend
{
    class LocalContextManager;
    class GlobalContext;

    class LocalContext
    {
    public:
        LocalContext(GlobalContext& globalContext);
        ~LocalContext();

        void Start();
        void Update();
        void Stop();

    private:
        ComponentManager mComponentManager;
        EventManager mEventManager;

        GlobalContext& mGlobalContext;

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

    class GlobalContext : public EventLayer
    {
    public:
        GlobalContext(const std::string& configPath);

        void Start();
        void Update();
        void Stop();

        template <typename T, typename... Args>
        T& NewNamedResource(const std::string& id, Args&... constructorParams)
        {
            auto resource = std::make_shared<T>(constructorParams...);
            mResourceRegistry.Add<T>(id, std::move(resource));

            return mResourceRegistry.Get<T>(id);
        }

        template <typename T, typename... Args>
        T& NewResource(Args&... constructorParams)
        {
            auto resource = std::make_shared<T>(constructorParams...);
            auto id = mResourceRegistry.Add<T>(std::move(resource));

            return mResourceRegistry.Get<T>(id);
        }

        Utilities::TOMLFile& GetGlobalSettings();

    private:
        EventManager mEventManager;
        Registry mResourceRegistry;
        LocalContextManager mLocalContextManager;
        Window mWindow;
        InputManager mInputManager;
        Renderer mRenderer;
        GlobalRenderGraph mGlobalRenderGraph;

        Utilities::TOMLFile mSettingsFile;

        void OnAppExit(const ApplicationQuitEvent& event);

        bool mRunning;

        friend class LocalContext;
        friend class Component;
        friend class Window;
        friend class WindowBackend;
    };
}