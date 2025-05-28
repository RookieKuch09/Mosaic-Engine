#pragma once

#include <functional>

namespace Mosaic::Frontend
{
    class EventManager;

    struct EventBase
    {
        virtual ~EventBase() = default;
    };

    template <typename T, typename U>
    struct Event : EventBase
    {
        Event(const T* origin, const T& data)
            : Origin(origin), Data(data)
        {
        }

        const T* Origin;
        U Data;
    };

    class EventLayer
    {
    public:
        EventLayer(EventManager& eventManager);
        virtual ~EventLayer();

        template <typename T, typename U>
        void Emit(const T* origin, const U& data);

        template <typename T, typename U>
        void SetCallback(const T* listener, void (T::*callback)(const Event<T, U>&));

        template <typename T, typename U>
        void RevokeCallback(const T* listener, void (T::*callback)(const Event<T, U>&));

        template <typename T, typename U>
        void RevokeCallbacks(const T* listener);
    };

    class EventManager
    {
    public:
        void Update();

    private:
        std::vector<std::reference_wrapper<EventBase>> mEventQueue;
    };
}