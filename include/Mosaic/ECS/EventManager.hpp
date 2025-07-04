#pragma once

#include <Mosaic/Application/Resources.hpp>

#include <any>
#include <functional>
#include <queue>
#include <typeindex>

namespace Mosaic
{
    class InstanceResources;

    class Application;

    template <typename... Components>
    class EntityView;

    template <typename Event, typename... Components>
    using EventResponder = std::function<void(InstanceResources&, const Event&, EntityView<Components...>)>;

    template <typename Event>
    using SimpleEventResponder = std::function<void(InstanceResources&, const Event&)>;

    class EventManager
    {
    public:
        template <typename Event, typename... Components>
        void AddResponder(const EventResponder<Event, Components...>& responder);

        template <typename Event>
        void AddResponder(const SimpleEventResponder<Event>& responder);

        template <typename Event, typename... Components>
        void AddResponder(void (*responder)(InstanceResources&, const Event&, EntityView<Components...>));

        template <typename Event>
        void AddResponder(void (*responder)(InstanceResources&, const Event&));

        template <typename T, typename Event>
        void AddResponder(T* instance, void (T::*responder)(InstanceResources&, const Event&));

        template <typename Event, typename... Args>
        void Emit(Args&&... args);

    private:
        using EventResponderAdapter = std::function<void(InstanceResources&, std::any& eventData)>;

        EventManager(InstanceResources& resources);

        template <typename Event, typename... Components>
        EventResponderAdapter MakeResponderAdapter(EventResponder<Event, Components...> responder);

        template <typename Event>
        EventResponderAdapter MakeResponderAdapter(SimpleEventResponder<Event> responder);

        void Update();

        template <typename T> requires std::is_base_of_v<Application, T>
        friend class Instance;

        InstanceResources& mInstanceResources;

        using EventQueueType = std::unordered_map<std::type_index, std::queue<std::any>>;
        using ResponderMapType = std::unordered_map<std::type_index, std::vector<EventResponderAdapter>>;

        EventQueueType mEventQueue;
        ResponderMapType mResponderMap;
    };
}

#include <Mosaic/Inline/ECS/EventManager.inl>