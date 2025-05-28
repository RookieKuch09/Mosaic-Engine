#pragma once

#include <functional>
#include <vector>

namespace Mosaic::Frontend
{
    class ComponentManager;

    class Component
    {
    public:
        Component(ComponentManager& componentManager);
        virtual ~Component();

        virtual void Start();
        virtual void Update();
        virtual void Close();

    private:
        ComponentManager& mComponentManager;

        bool mStarted;

        friend class ComponentManager;
    };

    class ComponentManager
    {
    public:
        void Start();
        void Update();
        void Close();

        void Register(Component* component);
        void Deregister(Component* component);

    private:
        void FlushQueuedStartComponents();
        void FlushQueuedCloseComponents();

        std::vector<std::reference_wrapper<Component>> mComponents;
        std::vector<std::reference_wrapper<Component>> mStartQueuedComponents;
        std::vector<std::reference_wrapper<Component>> mCloseQueuedComponents;
    };
}