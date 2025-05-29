#pragma once

#include "../utilities/toml.hpp"

#include "components.hpp"
#include "graphs.hpp"
#include "inputs.hpp"
#include "registry.hpp"
#include "renderer.hpp"
#include "window.hpp"

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
        void RemoveFromRegistry();

        ComponentManager mComponentManager;
        EventManager mEventManager;

        GlobalContext& mGlobalContext;

        bool mStarted;

        friend class LocalContextManager;
        friend class GlobalContext;
        friend class ComponentBase;
    };

    class LocalContextManager
    {
    public:
        void Start();
        void Update();
        void Stop();

        void Register(LocalContext* context);
        void Deregister(LocalContext* context);

        void Cleanup();

    private:
        void FlushQueuedStartContexts();
        void FlushQueuedStopContexts();

        std::vector<LocalContext*> mContexts;
        std::vector<LocalContext*> mStartQueuedContexts;
        std::vector<LocalContext*> mStopQueuedContexts;
    };

    class GlobalContext : public EventLayer
    {
    public:
        GlobalContext(const std::string& configPath);
        ~GlobalContext() override = default;

        void Start();
        void Update();
        void Stop();

        template <typename T, typename... Args>
        T& NewNamedResource(const std::string& id, Args&... constructorParams)
        {
            mResourceRegistry.Add<T>(id, constructorParams...);

            return mResourceRegistry.Get<T>(id);
        }

        template <typename T, typename... Args>
        T& NewResource(Args&... constructorParams)
        {
            auto id = mResourceRegistry.Add<T>(constructorParams...);

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
        friend class ComponentBase;
        friend class Window;
        friend class WindowBackend;
    };
}