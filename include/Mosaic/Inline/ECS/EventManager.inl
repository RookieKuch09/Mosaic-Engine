#pragma once

#include <Mosaic/ECS/EntityManager.hpp>
#include <Mosaic/ECS/EventManager.hpp>

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
    void EventManager::AddResponder(void (*responder)(InstanceResources&, const Event&, EntityView<Components...>))
    {
        AddResponder<Event, Components...>(EventResponder<Event, Components...>(responder));
    }

    template <typename Event>
    void EventManager::AddResponder(void (*responder)(InstanceResources&, const Event&))
    {
        AddResponder<Event>(EventResponder<Event>(responder));
    }

    template <typename T, typename Event>
    void EventManager::AddResponder(T* instance, void (T::*responder)(InstanceResources&, const Event&))
    {
        SimpleEventResponder<Event> bound = [instance, responder](InstanceResources& resources, const Event& event)
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
        return [userFn = std::move(responder), this](InstanceResources& resources, std::any& eventData)
        {
            const Event& event = std::any_cast<Event>(eventData);

            auto view = mInstanceResources.EntityManager.QueryView<Components...>();

            userFn(resources, event, view);
        };
    }

    template <typename Event>
    EventManager::EventResponderAdapter EventManager::MakeResponderAdapter(SimpleEventResponder<Event> responder)
    {
        return [userFn = std::move(responder)](InstanceResources& resources, std::any& eventData)
        {
            const Event& event = std::any_cast<const Event&>(eventData);

            userFn(resources, event);
        };
    }
}