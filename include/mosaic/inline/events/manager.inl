#pragma once

#include <mosaic/events/manager.hpp>

namespace Mosaic
{
    template <typename Event, typename... Components>
    void EventManager::AddResponder(const EventResponder<Event, Components...>& responder)
    {
        const std::type_index key = typeid(Event);

        mResponderMap[key].emplace_back(MakeResponderAdapter<Event, Components...>(responder));
    }

    template <typename Event>
    void EventManager::AddResponder(const SimpleEventResponder<Event>& responder)
    {
        const std::type_index key = typeid(Event);

        mResponderMap[key].emplace_back(MakeResponderAdapter<Event>(responder));
    }

    template <typename Event, typename... Components>
    void EventManager::AddResponder(void (*responder)(ApplicationResources&, const Event&, ECSView<Components...>))
    {
        AddResponder<Event, Components...>(EventResponder<Event, Components...>(responder));
    }

    template <typename Event>
    void EventManager::AddResponder(void (*responder)(ApplicationResources&, const Event&))
    {
        AddResponder<Event>(EventResponder<Event>(responder));
    }

    template <typename T, typename Event>
    void EventManager::AddResponder(T* instance, void (T::*responder)(ApplicationResources&, const Event&))
    {
        SimpleEventResponder<Event> bound = [instance, responder](ApplicationResources& resources, const Event& event)
        {
            (instance->*responder)(resources, event);
        };

        AddResponder<Event>(std::move(bound));
    }

    template <typename Event, typename... Args>
    void EventManager::Emit(Args&&... args)
    {
        static_assert(std::is_constructible_v<Event, Args...>, "Event must be constructible from provided arguments");

        const std::type_index key = typeid(Event);

        mEventQueue[key].emplace(std::make_any<Event>(Event(std::forward<Args>(args)...)));
    }

    template <typename Event, typename... Components>
    EventManager::EventResponderAdapter EventManager::MakeResponderAdapter(EventResponder<Event, Components...> responder)
    {
        return [userFn = std::move(responder), this](ApplicationResources& resources, std::any& eventData)
        {
            const Event& event = std::any_cast<Event>(eventData);

            auto view = mApplicationResources.ECSManager.QueryView<Components...>();

            userFn(resources, event, view);
        };
    }

    template <typename Event>
    EventManager::EventResponderAdapter EventManager::MakeResponderAdapter(SimpleEventResponder<Event> responder)
    {
        return [userFn = std::move(responder)](ApplicationResources& resources, std::any& eventData)
        {
            const Event& event = std::any_cast<const Event&>(eventData);

            userFn(resources, event);
        };
    }
}