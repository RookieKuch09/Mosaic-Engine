#include "../../include/frontend/components.hpp"
#include "../../include/frontend/contexts.hpp"
#include "../../include/frontend/logging.hpp"

namespace Mosaic::Frontend
{
    Component::Component(LocalContext& context)
        : mLocalContext(context), ResourceRegistry(context.mGlobalContext.mResourceRegistry), EventLayer(context.mEventManager, context.mGlobalContext.mEventManager), mStarted(false)
    {
        mLocalContext.mComponentManager.Register(this);
    }

    Component::~Component()
    {
        mLocalContext.mComponentManager.Deregister(this);
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

    void ComponentManager::Start()
    {
        FlushQueuedStartComponents();
        FlushQueuedStopComponents();

        for (const auto& component : mComponents)
        {
            auto& cmp = component.get();

            cmp.mStarted = true;
            cmp.Start();
        }
    }

    void ComponentManager::Update()
    {
        FlushQueuedStartComponents();
        FlushQueuedStopComponents();

        for (const auto& component : mComponents)
        {
            auto& cmp = component.get();

            if (not cmp.mStarted)
            {
                cmp.mStarted = true;
                cmp.Start();
            }

            cmp.Update();
        }
    }

    void ComponentManager::Stop()
    {
        for (const auto& component : mComponents)
        {
            auto& cmp = component.get();

            if (cmp.mStarted)
            {
                cmp.Stop();
            }
        }
    }

    void ComponentManager::Register(Component* component)
    {
        mComponents.push_back(std::ref(*component));
    }

    void ComponentManager::Deregister(Component* logicContext)
    {
        mStopQueuedComponents.push_back(std::ref(*logicContext));
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

        removeContext(mComponents, mStopQueuedComponents, "Component");
    }
}