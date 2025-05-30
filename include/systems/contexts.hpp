#pragma once

#include "../utilities/toml.hpp"

#include "components.hpp"
#include "graphs.hpp"
#include "inputs.hpp"
#include "renderer.hpp"
#include "window.hpp"

namespace Mosaic::Systems
{
    class LocalContextManager;
    class GlobalContext;

    class LocalContext
    {
    public:
        LocalContext(GlobalContext& globalContext);
        ~LocalContext();

        LocalContext(const LocalContext&) = delete;
        LocalContext& operator=(const LocalContext&) = delete;

        LocalContext(LocalContext&&) noexcept;
        LocalContext& operator=(LocalContext&&) noexcept;

        void NewComponent(const std::string& tag);

        template <typename T, typename... Args>
        inline void NewComponent(const std::string& tag, Args&&... args)
        {
            mComponentManager.NewComponent<T>(tag, std::forward<Args>(args)...);
        }

        template <typename T>
        inline T& GetComponent(const std::string& tag)
        {
            return mComponentManager.GetComponentInstance<T>(tag);
        }

        template <typename T>
        inline std::string GetComponentTag(T& component)
        {
            return mComponentManager.GetComponentTag(component);
        }

        template <typename T>
        inline void RemoveComponent(const std::string& tag)
        {
            mComponentManager.RemoveComponentByTag<T>(tag);
        }

        template <typename T>
        inline void RemoveComponent(T& component)
        {
            mComponentManager.RemoveComponentByInstance(component);
        }

    private:
        void Start();
        void Update();
        void Stop();

        ComponentManager mComponentManager;
        EventManager mEventManager;

        GlobalContext& mGlobalContext;

        bool mStarted;

        friend class LocalContextManager;
        friend class Component;
    };

    class LocalContextManager
    {
    public:
        LocalContext& NewContext(const std::string& tag);

        LocalContext& GetContext(const std::string& tag);

        std::string GetContextTag(LocalContext& context);

        void RemoveContextByTag(const std::string& tag);

        void RemoveContextByInstance(LocalContext& context);

    private:
        LocalContextManager(GlobalContext& globalContext);

        void Start();
        void Update();
        void Stop();

        void FlushQueuedStartContexts();
        void FlushQueuedStopContexts();

        GlobalContext& mGlobalContext;

        std::vector<LocalContext> mOwnedContexts;
        std::vector<LocalContext> mPendingContexts;

        std::vector<LocalContext*> mContexts;
        std::vector<LocalContext*> mStartQueue;
        std::vector<LocalContext*> mStopQueue;

        std::unordered_map<std::string, LocalContext*> mContextLookup;

        friend class LocalContext;
        friend class GlobalContext;
    };

    class GlobalContext : public EventLayer
    {
    public:
        ~GlobalContext() override = default;

        LocalContext& NewContext(const std::string& tag);

        LocalContext& GetContext(const std::string& tag);

        std::string GetContextTag(LocalContext& context);

        void RemoveContextByTag(const std::string& tag);

        void RemoveContextByInstance(LocalContext& context);

    private:
        GlobalContext(const std::string& configPath);

        void Start();
        void Update();
        void Stop();

        Utilities::TOMLFile& GetGlobalSettings();

        void OnAppExit(const ApplicationQuitEvent& event);

        EventManager mEventManager;
        LocalContextManager mLocalContextManager;
        Window mWindow;
        InputManager mInputManager;
        Renderer mRenderer;
        GlobalRenderGraph mGlobalRenderGraph;

        Utilities::TOMLFile mSettingsFile;

        bool mRunning;

        friend class LocalContext;
        friend class Component;
        friend class Window;
        friend class WindowBackend;
        friend class Instance;
    };
}