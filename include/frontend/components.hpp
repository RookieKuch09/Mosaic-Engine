#pragma once

#include "events.hpp"

#include <functional>
#include <vector>

namespace Mosaic::Frontend
{
    class ComponentManager;
    class Registry;
    class LocalContext;

    class Component : public EventLayer
    {
    public:
        Component(LocalContext& logicContext);
        virtual ~Component();

        virtual void Start();
        virtual void Update();
        virtual void Stop();

    protected:
        Registry& ResourceRegistry;

    private:
        LocalContext& mLocalContext;

        bool mStarted;

        friend class ComponentManager;
    };

    class ComponentManager
    {
    public:
        void Start();
        void Update();
        void Stop();

        void Register(Component* component);
        void Deregister(Component* component);

    private:
        void FlushQueuedStartComponents();
        void FlushQueuedStopComponents();

        std::vector<std::reference_wrapper<Component>> mComponents;
        std::vector<std::reference_wrapper<Component>> mStartQueuedComponents;
        std::vector<std::reference_wrapper<Component>> mStopQueuedComponents;
    };
}