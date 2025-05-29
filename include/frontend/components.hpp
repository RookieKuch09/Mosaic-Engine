#pragma once

#include "events.hpp"
#include "registry.hpp"

#include <vector>

namespace Mosaic::Frontend
{
    class ComponentManager;
    class LocalContext;
    class GlobalContext;

    class ComponentBase : public EventLayer
    {
    public:
        ComponentBase(LocalContext& localContext);
        virtual ~ComponentBase();

        virtual void Start() = 0;
        virtual void Update() = 0;
        virtual void Stop() = 0;

        virtual void RemoveFromRegistry() = 0;

    protected:
        Registry& ResourceRegistry;

    private:
        LocalContext& mLocalContext;

        bool mStarted;

        friend class ComponentManager;
    };

    template <typename Derived>
    class Component : public ComponentBase
    {
    public:
        Component(LocalContext& localContext)
            : ComponentBase(localContext)
        {
        }

        virtual ~Component() override = default;

        virtual void Start() override
        {
        }

        virtual void Update() override
        {
        }

        virtual void Stop() override
        {
        }

        void RemoveFromRegistry() override
        {
            Utilities::LogNotice("Deleting Component");
            ResourceRegistry.Remove<Derived>(static_cast<Derived&>(*this));
        }
    };

    class ComponentManager
    {
    public:
        void Start();
        void Update();
        void Stop();

        void Register(ComponentBase* component);
        void Deregister(ComponentBase* component);

        void Cleanup();

    private:
        void FlushQueuedStartComponents();
        void FlushQueuedStopComponents();

        std::vector<ComponentBase*> mComponents;
        std::vector<ComponentBase*> mStartQueuedComponents;
        std::vector<ComponentBase*> mStopQueuedComponents;
    };
}