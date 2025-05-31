#pragma once

#include "../utilities/logging.hpp"

#include <boost/type_index.hpp>

#include <algorithm>
#include <any>
#include <functional>
#include <queue>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace Mosaic::Systems
{
    struct ApplicationQuitEvent
    {
    };

    class EventManager;

    class EventManager
    {
    public:
        void Update();

    private:
        template <typename T, typename TClass>
        inline void SetCallback(TClass* subscriber, void (TClass::*callback)(const T&))
        {
            auto call = [subscriber, callback](const std::any& event)
            {
                (subscriber->*callback)(std::any_cast<const T&>(event));
            };

            auto& list = mListeners[typeid(T)];

            auto it = std::find_if(list.begin(), list.end(),
                                   [subscriber](const EventListener& l)
                                   { return l.Subscriber == subscriber; });
            if (it == list.end())
            {
                list.push_back({subscriber, call});
            }
            else
            {
                Utilities::Throw("Subscriber {} already subscribed to event {}", boost::typeindex::type_id<TClass>().pretty_name(), boost::typeindex::type_id<T>().pretty_name());
            }
        }

        void RevokeCallbacks(void* subscriber);

        template <typename T, typename... Args>
        inline void PostEvent(Args&&... args)
        {
            mEventQueue[typeid(T)].emplace(T(std::forward<Args>(args)...));
        }

        struct EventListener
        {
            void* Subscriber;

            std::function<void(const std::any&)> Callback;
        };

        std::unordered_map<std::type_index, std::queue<std::any>> mEventQueue;
        std::unordered_map<std::type_index, std::vector<EventListener>> mListeners;

        friend class EventLayer;
    };

    class EventLayer
    {
    public:
        EventLayer(EventManager& eventManager, EventManager& globalEventManager);
        virtual ~EventLayer() = default;

        template <typename T, typename TClass>
        inline void SetLocalCallback(TClass* subscriber, void (TClass::*callback)(const T&))
        {
            mLocalEventManager.SetCallback(subscriber, callback);
        }

        void RevokeLocalCallbacks(void* subscriber);

        template <typename T, typename... Args>
        inline void PostLocalEvent(Args&&... args)
        {
            mLocalEventManager.PostEvent<T>(std::forward<Args>(args)...);
        }

        template <typename T, typename TClass>
        inline void SetGlobalCallback(TClass* subscriber, void (TClass::*callback)(const T&))
        {
            mGlobalEventManager.SetCallback(subscriber, callback);
        }

        void RevokeGlobalCallbacks(void* subscriber);

        template <typename T, typename... Args>
        inline void PostGlobalEvent(Args&&... args)
        {
            mGlobalEventManager.PostEvent<T>(std::forward<Args>(args)...);
        }

    private:
        EventManager& mLocalEventManager;
        EventManager& mGlobalEventManager;
    };
}