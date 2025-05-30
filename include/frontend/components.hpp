#pragma once

#include "events.hpp"

#include <vector>

namespace Mosaic::Frontend
{
    class ComponentManager;
    class LocalContext;
    class GlobalContext;

    class Component : public EventLayer
    {
    public:
        virtual ~Component();

    protected:
        Component(LocalContext& localContext, GlobalContext& globalContext);

        virtual void Start();
        virtual void Update();
        virtual void Stop();

        EventManager& LocalEventManager;
        EventManager& GlobalEventManager;

        LocalContext& LocalContext;
        GlobalContext& GlobalContext;

    private:
        bool mStarted;

        friend class ComponentManager;
    };

    class ComponentManager
    {
    public:
        ComponentManager(ComponentManager&& other) noexcept = default;
        ComponentManager& operator=(ComponentManager&& other) noexcept;

        template <typename T, typename... Args>
        inline T& NewComponent(const std::string& tag, Args&&... args)
        {
            static_assert(std::is_base_of_v<Component, T>, "Type provided must inherit from Component");

            auto component = std::make_unique<T>(mLocalContext, mGlobalContext, std::forward<Args>(args)...);
            T* rawPtr = component.get();

            mStartQueue.push_back(rawPtr);
            mPendingComponents.emplace_back(std::move(component));

            mComponentLookup[tag] = rawPtr;

            return *rawPtr;
        }

        template <typename T>
        inline T& GetComponentInstance(const std::string& tag)
        {
            static_assert(std::is_base_of_v<Component, T>, "Type provided must inherit from Component");

            auto it = mComponentLookup.find(tag);

            if (it == mComponentLookup.end() or not it->second)
            {
                Utilities::Throw("No Component with tag {} of type {} found", boost::typeindex::type_id<T>().pretty_name(), tag);
            }

            T* casted = dynamic_cast<T*>(it->second);

            if (not casted)
            {
                Utilities::Throw("Component with tag {} not of requested type {}", tag, boost::typeindex::type_id<T>().pretty_name());
            }

            return *casted;
        }

        template <typename T>
        inline std::string GetComponentTag(T& component)
        {
            static_assert(std::is_base_of_v<Component, T>, "Type provided must inherit from Component");

            for (const auto& [tag, ptr] : mComponentLookup)
            {
                if (ptr == &component)
                {
                    return tag;
                }
            }

            Utilities::Throw("Component not registered with the manager");
        }

        template <typename T>
        inline void RemoveComponentByTag(const std::string& tag)
        {
            static_assert(std::is_base_of_v<Component, T>, "Type provided must inherit from Component");

            auto it = mComponentLookup.find(tag);

            if (it == mComponentLookup.end() or not it->second)
            {
                Utilities::Throw("No Component with tag {} of type {} found", tag, boost::typeindex::type_id<T>().pretty_name());
            }

            T* comp = dynamic_cast<T*>(it->second);

            if (not comp)
            {
                Utilities::Throw("Component with tag {} not of requested type {}", tag, boost::typeindex::type_id<T>().pretty_name());
            }

            mStopQueue.push_back(comp);
        }

        template <typename T>
        inline void RemoveComponentByInstance(T& component)
        {
            static_assert(std::is_base_of_v<Component, T>, "Type provided must inherit from Component");

            auto match = [&](const std::unique_ptr<Component>& ptr)
            {
                return ptr.get() == &component;
            };

            auto owned = std::find_if(mOwnedComponents.begin(), mOwnedComponents.end(), match);

            if (owned != mOwnedComponents.end())
            {
                mStopQueue.push_back(&component);
            }
            else
            {
                Utilities::Throw("Attempt to remove a component of type {} not owned by this ComponentManager", boost::typeindex::type_id<T>().pretty_name());
            }
        }

    private:
        ComponentManager(LocalContext& localContext, GlobalContext& globalContext);

        void Start();
        void Update();
        void Stop();

        void FlushQueuedStartComponents();
        void FlushQueuedStopComponents();

        LocalContext& mLocalContext;
        GlobalContext& mGlobalContext;

        std::vector<std::unique_ptr<Component>> mOwnedComponents;
        std::vector<std::unique_ptr<Component>> mPendingComponents;

        std::vector<Component*> mComponents;
        std::vector<Component*> mStartQueue;
        std::vector<Component*> mStopQueue;

        std::unordered_map<std::string, Component*> mComponentLookup;

        friend class LocalContext;
    };
}