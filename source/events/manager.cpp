#include <mosaic/events/manager.hpp>

namespace Mosaic
{
    EventManager::EventManager(Resources& resources)
        : mResources(resources)
    {
    }

    void EventManager::Update()
    {
        EventQueueType eventsToProcess = std::move(mEventQueue);

        mEventQueue.clear();

        for (auto& [eventType, eventQueue] : eventsToProcess)
        {
            auto responderIt = mResponderMap.find(eventType);

            if (responderIt == mResponderMap.end())
            {
                continue;
            }

            auto& responders = responderIt->second;

            while (not eventQueue.empty())
            {
                std::any& storedEvent = eventQueue.front();

                for (auto& responder : responders)
                {
                    responder(mResources, storedEvent);
                }

                eventQueue.pop();
            }
        }
    }
}
