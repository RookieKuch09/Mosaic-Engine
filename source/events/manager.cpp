#include <mosaic/events/manager.hpp>

namespace Mosaic
{
    EventManager::EventManager(ApplicationResources& resources)
        : mApplicationResources(resources)
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
                    responder(mApplicationResources, storedEvent);
                }

                eventQueue.pop();
            }
        }
    }
}
