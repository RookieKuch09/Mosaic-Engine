#include "../../include/systems/events.hpp"

#include <algorithm>

namespace Mosaic::Systems
{
    void EventManager::Update()
    {
        for (auto event = mEventQueue.begin(); event != mEventQueue.end();)
        {
            auto eventListeners = mListeners.find(event->first);

            if (eventListeners != mListeners.end())
            {
                auto& queue = event->second;
                auto& listeners = eventListeners->second;

                while (not queue.empty())
                {
                    const std::any& event = queue.front();

                    for (auto& listener : listeners)
                    {
                        listener.Callback(event);
                    }

                    queue.pop();
                }
            }

            event = mEventQueue.erase(event);
        }
    }

    void EventManager::RevokeCallbacks(void* subscriber)
    {
        auto condition = [&](const EventListener& listener)
        {
            return listener.Subscriber == subscriber;
        };

        for (auto& [event, subscriptions] : mListeners)
        {
            subscriptions.erase(std::remove_if(subscriptions.begin(), subscriptions.end(), condition), subscriptions.end());
        }
    }

    EventLayer::EventLayer(EventManager& localEventManager, EventManager& globalEventManager)
        : mLocalEventManager(localEventManager), mGlobalEventManager(globalEventManager)
    {
    }

    void EventLayer::RevokeLocalCallbacks(void* subscriber)
    {
        mLocalEventManager.RevokeCallbacks(subscriber);
    }

    void EventLayer::RevokeGlobalCallbacks(void* subscriber)
    {
        mGlobalEventManager.RevokeCallbacks(subscriber);
    }
}