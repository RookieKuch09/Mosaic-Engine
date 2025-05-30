#include "../../include/systems/components.hpp"
#include "../../include/systems/contexts.hpp"

namespace Mosaic::Systems
{
    Component::Component(Mosaic::Systems::LocalContext& localContext, Mosaic::Systems::GlobalContext& globalContext)
        : EventLayer(localContext.mEventManager, globalContext.mEventManager), LocalContext(localContext), GlobalContext(globalContext), LocalEventManager(localContext.mEventManager), GlobalEventManager(globalContext.mEventManager), Window(globalContext.mWindow), Renderer(globalContext.mRenderer)
    {
    }

    Component::~Component()
    {
    }

    void Component::Start()
    {
    }

    void Component::Update()
    {
    }

    void Component::Stop()
    {
    }

    ComponentManager& ComponentManager::operator=(ComponentManager&& other) noexcept
    {
        if (this != &other)
        {
            mOwnedComponents = std::move(other.mOwnedComponents);
            mPendingComponents = std::move(other.mPendingComponents);
            mComponents = std::move(other.mComponents);
            mStartQueue = std::move(other.mStartQueue);
            mStopQueue = std::move(other.mStopQueue);
            mComponentLookup = std::move(other.mComponentLookup);
        }

        return *this;
    }

    ComponentManager::ComponentManager(LocalContext& localContext, GlobalContext& globalContext)
        : mGlobalContext(globalContext), mLocalContext(localContext)
    {
    }

    void ComponentManager::Start()
    {
        FlushQueuedStartComponents();
        FlushQueuedStopComponents();
    }

    void ComponentManager::Update()
    {
        FlushQueuedStartComponents();
        FlushQueuedStopComponents();

        for (auto* component : mComponents)
        {
            component->Update();
        }
    }

    void ComponentManager::Stop()
    {
        for (auto* component : mComponents)
        {
            mStopQueue.push_back(component);
        }

        FlushQueuedStopComponents();
    }

    void ComponentManager::FlushQueuedStartComponents()
    {
        for (auto* component : mStartQueue)
        {
            component->Start();
            mComponents.push_back(component);
        }

        mStartQueue.clear();

        for (auto& owned : mPendingComponents)
        {
            mOwnedComponents.emplace_back(std::move(owned));
        }

        mPendingComponents.clear();
    }

    void ComponentManager::FlushQueuedStopComponents()
    {
        for (auto* component : mStopQueue)
        {
            auto checkPair = [&](const auto& pair)
            {
                return pair.second == component;
            };

            auto checkPtr = [&](const std::unique_ptr<Component>& ptr)
            {
                return ptr.get() == component;
            };

            component->Stop();

            mComponents.erase(std::remove(mComponents.begin(), mComponents.end(), component), mComponents.end());

            auto it = std::find_if(mComponentLookup.begin(), mComponentLookup.end(), checkPair);

            if (it != mComponentLookup.end())
            {
                mComponentLookup.erase(it);
            }

            mOwnedComponents.erase(std::remove_if(mOwnedComponents.begin(), mOwnedComponents.end(), checkPtr), mOwnedComponents.end());
        }

        mStopQueue.clear();
    }
}