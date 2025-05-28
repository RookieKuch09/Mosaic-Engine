#include <frontend/components.hpp>
#include <frontend/logging.hpp>

namespace Mosaic::Frontend
{
    Component::Component(ComponentManager& componentManager)
        : mComponentManager(componentManager), mStarted(false)
    {
        mComponentManager.Register(this);
    }

    Component::~Component()
    {
        mComponentManager.Deregister(this);
    }

    void Component::Start()
    {
    }

    void Component::Update()
    {
    }

    void Component::Close()
    {
    }

    void ComponentManager::Start()
    {
        FlushQueuedStartComponents();
        FlushQueuedCloseComponents();

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
        FlushQueuedCloseComponents();

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

    void ComponentManager::Close()
    {
        for (const auto& component : mComponents)
        {
            auto& cmp = component.get();

            if (cmp.mStarted)
            {
                cmp.Close();
            }
        }
    }

    void ComponentManager::Register(Component* component)
    {
        mComponents.push_back(std::ref(*component));
    }

    void ComponentManager::Deregister(Component* logicContext)
    {
        mCloseQueuedComponents.push_back(std::ref(*logicContext));
    }

    void ComponentManager::FlushQueuedStartComponents()
    {
        std::move(mStartQueuedComponents.begin(), mStartQueuedComponents.end(), std::back_inserter(mComponents));

        mStartQueuedComponents.clear();
    }

    void ComponentManager::FlushQueuedCloseComponents()
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

        removeContext(mComponents, mCloseQueuedComponents, "Component");
    }
}