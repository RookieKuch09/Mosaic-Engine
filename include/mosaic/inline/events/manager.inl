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
    void EventManager::AddResponder(void (*responder)(Resources&, const Event&, ECSView<Components...>))
    {
        AddResponder<Event, Components...>(EventResponder<Event, Components...>(responder));
    }

    template <typename Event>
    void EventManager::AddResponder(void (*responder)(Resources&, const Event&))
    {
        AddResponder<Event>(EventResponder<Event>(responder));
    }

    template <typename T, typename Event>
    void EventManager::AddResponder(T* instance, void (T::*responder)(Resources&, const Event&))
    {
        SimpleEventResponder<Event> bound = [instance, responder](Resources& resources, const Event& event)
        {
            (instance->*responder)(resources, event);
        };

        AddResponder<Event>(std::move(bound));
    }

    template <typename Event>
    void EventManager::Emit(const Event& event)
    {
        const std::type_index key = typeid(Event);

        mEventQueue[key].emplace(std::make_any<Event>(event));
    }

    template <typename Event, typename... Components>
    auto EventManager::MakeResponderAdapter(EventResponder<Event, Components...> responder) -> EventResponderAdapter
    {
        return [userFn = std::move(responder), this](Resources& resources, std::any& eventData)
        {
            const Event& event = std::any_cast<Event>(eventData);

            auto view = mResources.ECSManager.QueryView<Components...>();

            userFn(resources, event, view);
        };
    }

    template <typename Event>
    auto EventManager::MakeResponderAdapter(SimpleEventResponder<Event> responder) -> EventResponderAdapter
    {
        return [userFn = std::move(responder)](Resources& resources, std::any& eventData)
        {
            const Event& event = std::any_cast<const Event&>(eventData);

            userFn(resources, event);
        };
    }
}