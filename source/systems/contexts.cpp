#include "../../include/systems/contexts.hpp"

namespace Mosaic::Systems
{
    LocalContext::LocalContext(GlobalContext& globalContext)
        : mGlobalContext(globalContext), mComponentManager(*this, globalContext), mStarted(false)
    {
    }

    LocalContext::LocalContext(LocalContext&& other) noexcept
        : mComponentManager(std::move(other.mComponentManager)), mEventManager(std::move(other.mEventManager)), mGlobalContext(other.mGlobalContext), mStarted(other.mStarted)
    {
        other.mStarted = false;
    }

    LocalContext& LocalContext::operator=(LocalContext&& other) noexcept
    {
        if (this != &other)
        {
            mComponentManager = std::move(other.mComponentManager);
            mEventManager = std::move(other.mEventManager);

            mStarted = other.mStarted;
            other.mStarted = false;
        }
        return *this;
    }

    LocalContext::~LocalContext()
    {
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

    LocalContextManager::LocalContextManager(GlobalContext& globalContext)
        : mGlobalContext(globalContext)
    {
    }

    LocalContext& LocalContextManager::NewContext(const std::string& tag)
    {
        if (mContextLookup.find(tag) != mContextLookup.end())
        {
            Utilities::Throw("LocalContext with tag '{}' already exists", tag);
        }

        mPendingContexts.emplace_back(mGlobalContext);
        LocalContext& context = mPendingContexts.back();

        mStartQueue.push_back(&context);
        mContextLookup[tag] = &context;

        return context;
    }

    LocalContext& LocalContextManager::GetContext(const std::string& tag)
    {
        auto it = mContextLookup.find(tag);

        if (it == mContextLookup.end() or not it->second)
        {
            Utilities::Throw("No LocalContext with tag {} found", tag);
        }

        return *it->second;
    }

    std::string LocalContextManager::GetContextTag(LocalContext& context)
    {
        for (const auto& [tag, ptr] : mContextLookup)
        {
            if (ptr == &context)
            {
                return tag;
            }
        }

        Utilities::Throw("LocalContext not registered with the manager");
    }

    void LocalContextManager::RemoveContextByTag(const std::string& tag)
    {
        auto it = mContextLookup.find(tag);

        if (it == mContextLookup.end() or not it->second)
        {
            Utilities::Throw("No LocalContext with tag {} found", tag);
        }

        LocalContext* context = it->second;

        mStopQueue.push_back(context);
    }

    void LocalContextManager::RemoveContextByInstance(LocalContext& context)
    {
        auto match = [&](const LocalContext& c)
        {
            return &c == &context;
        };

        auto owned = std::find_if(mOwnedContexts.begin(), mOwnedContexts.end(), match);

        if (owned != mOwnedContexts.end())
        {
            mStopQueue.push_back(&context);
        }
        else
        {
            Utilities::Throw("Attempt to remove a context not owned by this LocalContextManager");
        }
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

    void LocalContextManager::FlushQueuedStartContexts()
    {
        for (auto& context : mStartQueue)
        {
            context->Start();
            mContexts.push_back(context);
        }

        mStartQueue.clear();

        for (auto& owned : mPendingContexts)
        {
            mOwnedContexts.emplace_back(std::move(owned));
        }

        mPendingContexts.clear();
    }

    void LocalContextManager::FlushQueuedStopContexts()
    {
        for (auto& context : mStopQueue)
        {
            auto checkPair = [&](auto& pair)
            {
                return pair.second == context;
            };

            auto checkRef = [&](LocalContext& ref)
            {
                return &ref == context;
            };

            context->Stop();

            mContexts.erase(std::remove(mContexts.begin(), mContexts.end(), context), mContexts.end());

            auto it = std::find_if(mContextLookup.begin(), mContextLookup.end(), checkPair);

            if (it != mContextLookup.end())
            {
                mContextLookup.erase(it);
            }

            mOwnedContexts.erase(std::remove_if(mOwnedContexts.begin(), mOwnedContexts.end(), checkRef), mOwnedContexts.end());
        }

        mStopQueue.clear();
    }

    LocalContext& GlobalContext::NewContext(const std::string& id)
    {
        return mLocalContextManager.NewContext(id);
    }

    LocalContext& GlobalContext::GetContext(const std::string& tag)
    {
        return mLocalContextManager.GetContext(tag);
    }

    std::string GlobalContext::GetContextTag(LocalContext& context)
    {
        return mLocalContextManager.GetContextTag(context);
    }

    void GlobalContext::RemoveContextByTag(const std::string& tag)
    {
        mLocalContextManager.RemoveContextByTag(tag);
    }

    void GlobalContext::RemoveContextByInstance(LocalContext& context)
    {
        mLocalContextManager.RemoveContextByInstance(context);
    }

    GlobalContext::GlobalContext(const std::string& configPath)
        : EventLayer(mEventManager, mEventManager), mRunning(true), mSettingsFile(configPath), mLocalContextManager(*this), mWindow(*this)
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
        SetGlobalCallback<ApplicationQuitEvent>(this, &GlobalContext::OnAppExit);

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