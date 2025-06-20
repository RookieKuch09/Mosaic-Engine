#pragma once

#include <mosaic/application/resources.hpp>

#include <mosaic/ecs/manager.hpp>

#include <any>
#include <functional>
#include <queue>

namespace Mosaic
{
    struct Resources;

    class Application;

    template <typename... Components>
    class ECSView;

    template <typename T> requires std::is_base_of_v<Application, T>
    class Instance;

    template <typename Event, typename... Components>
    using EventResponder = std::function<void(Resources&, const Event&, ECSView<Components...>)>;

    template <typename Event>
    using SimpleEventResponder = std::function<void(Resources&, const Event&)>;

    class EventManager
    {
    public:
        template <typename Event, typename... Components>
        void AddResponder(const EventResponder<Event, Components...>& responder);

        template <typename Event>
        void AddResponder(const SimpleEventResponder<Event>& responder);

        template <typename Event, typename... Components>
        void AddResponder(void (*responder)(Resources&, const Event&, ECSView<Components...>));

        template <typename Event>
        void AddResponder(void (*responder)(Resources&, const Event&));

        template <typename T, typename Event>
        void AddResponder(T* instance, void (T::*responder)(Resources&, const Event&));

        template <typename Event>
        void Emit(const Event& event);

    private:
        using EventResponderAdapter = std::function<void(Resources&, std::any& eventData)>;

        EventManager(Resources& resources);

        template <typename Event, typename... Components>
        EventResponderAdapter MakeResponderAdapter(EventResponder<Event, Components...> responder);

        template <typename Event>
        EventResponderAdapter MakeResponderAdapter(SimpleEventResponder<Event> responder);

        void Update();

        template <typename T> requires std::is_base_of_v<Application, T>
        friend class Instance;

        Resources& mResources;

        using EventQueueType = std::unordered_map<std::type_index, std::queue<std::any>>;
        using ResponderMapType = std::unordered_map<std::type_index, std::vector<EventResponderAdapter>>;

        EventQueueType mEventQueue;
        ResponderMapType mResponderMap;
    };
}

#include <mosaic/inline/events/manager.inl>