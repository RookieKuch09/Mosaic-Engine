#include "../../include/frontend/components.hpp"
#include "../../include/frontend/contexts.hpp"

#include "../../include/utilities/logging.hpp"

namespace Mosaic::Frontend
{
    ComponentBase::ComponentBase(LocalContext& localContext)
        : mLocalContext(localContext), ResourceRegistry(localContext.mGlobalContext.mResourceRegistry), EventLayer(localContext.mEventManager, localContext.mGlobalContext.mEventManager), mStarted(false)
    {
        mLocalContext.mComponentManager.Register(this);
    }

    ComponentBase::~ComponentBase()
    {
        Utilities::LogNotice("Deleting ComponentBase");
        mLocalContext.mComponentManager.Deregister(this);
    }

    void ComponentManager::Start()
    {
        FlushQueuedStartComponents();
        FlushQueuedStopComponents();

        for (const auto& component : mComponents)
        {
            component->mStarted = true;
            component->Start();
        }
    }

    void ComponentManager::Update()
    {
        FlushQueuedStartComponents();
        FlushQueuedStopComponents();

        for (const auto& component : mComponents)
        {
            if (not component->mStarted)
            {
                component->mStarted = true;
                component->Start();
            }

            component->Update();
        }
    }

    void ComponentManager::Stop()
    {
        FlushQueuedStartComponents();
        FlushQueuedStopComponents();

        for (const auto& component : mComponents)
        {
            if (component->mStarted)
            {
                component->Stop();
            }
        }
    }

    void ComponentManager::Register(ComponentBase* component)
    {
        mComponents.push_back(component);
    }

    void ComponentManager::Deregister(ComponentBase* component)
    {
        mStopQueuedComponents.push_back(component);
    }

    void ComponentManager::Cleanup()
    {

        for (ComponentBase* component : mComponents)
        {
            if (component)
            {
                component->RemoveFromRegistry();
            }
        }

        for (ComponentBase* component : mStartQueuedComponents)
        {
            if (component)
            {
                component->RemoveFromRegistry();
            }
        }

        for (ComponentBase* component : mStopQueuedComponents)
        {
            if (component)
            {
                component->RemoveFromRegistry();
            }
        }
    }

    void ComponentManager::FlushQueuedStartComponents()
    {
        std::move(mStartQueuedComponents.begin(), mStartQueuedComponents.end(), std::back_inserter(mComponents));
        mStartQueuedComponents.clear();
    }

    void ComponentManager::FlushQueuedStopComponents()
    {
        auto removeContext = [](auto& vec, auto& queue, const char* contextType)
        {
            for (const auto* component : queue)
            {
                auto it = std::find(vec.begin(), vec.end(), component);

                if (it != vec.end())
                {
                    vec.erase(it);
                }
                else
                {
                    Utilities::Throw("{} is not registered and cannot be deregistered", contextType);
                }
            }

            queue.clear();
        };

        removeContext(mComponents, mStopQueuedComponents, "Component");
    }
}