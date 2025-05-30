#pragma once

#include <any>
#include <functional>
#include <queue>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace Mosaic::Systems::Internal
{
    struct EventListener
    {
        void* Subscriber;

        std::function<void(const std::any&)> Callback;
    };

    class EventManager
    {
    public:
        template <typename T>
        void Subscribe(void* subscriber, std::function<void(const T&)> callback);

        template <typename T, typename TClass>
        void Subscribe(TClass* subscriber, void (TClass::*callback)(const T&));

        void Unsubscribe(void* subscriber);

        template <typename T>
        void Unsubscribe(void* subscriber);

        template <typename T>
        void Emit(const T& event);

    private:
        std::unordered_map<std::type_index, std::queue<std::any>> mEventQueue;
        std::unordered_map<std::type_index, std::vector<EventListener>> mListeners;

        void Update();

        friend class Application;
    };
}

#include "application/events.inl"