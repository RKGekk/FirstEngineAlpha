#include "EventManager.h"

#include <windows.h>

EventManager::EventManager(const char* pName, bool setAsGlobal) : IEventManager(pName, setAsGlobal) {
    m_activeQueue = 0;
}

EventManager::~EventManager() {}

bool EventManager::VAddListener(const fastdelegate::FastDelegate1<std::shared_ptr<IEventData>>& eventDelegate, const unsigned long& type) {
    auto& eventListenerList = m_eventListeners[type];  // this will find or create the entry
    for (auto it = eventListenerList.begin(); it != eventListenerList.end(); ++it) {
        if (eventDelegate == (*it)) {
            return false;
        }
    }
    eventListenerList.push_back(eventDelegate);
    return true;
}

bool EventManager::VRemoveListener(const fastdelegate::FastDelegate1<std::shared_ptr<IEventData>>& eventDelegate, const unsigned long& type) {
    bool success = false;
    auto findIt = m_eventListeners.find(type);
    if (findIt != m_eventListeners.end()) {
        auto& listeners = findIt->second;
        for (auto it = listeners.begin(); it != listeners.end(); ++it) {
            if (eventDelegate == (*it)) {
                listeners.erase(it);
                success = true;
                break;  // we don't need to continue because it should be impossible for the same delegate function to be registered for the same event more than once
            }
        }
    }
    return success;
}

bool EventManager::VTriggerEvent(const std::shared_ptr<IEventData>& pEvent) const {
    bool processed = false;
    auto findIt = m_eventListeners.find(pEvent->VGetEventType());
    if (findIt != m_eventListeners.end()) {
        const auto& eventListenerList = findIt->second;
        for (auto it = eventListenerList.begin(); it != eventListenerList.end(); ++it) {
            auto listener = (*it);
            listener(pEvent);  // call the delegate
            processed = true;
        }
    }

    return processed;
}

bool EventManager::VQueueEvent(const std::shared_ptr<IEventData>& pEvent) {
    // make sure the event is valid
    if (!pEvent) {
        return false;
    }

    auto findIt = m_eventListeners.find(pEvent->VGetEventType());
    if (findIt != m_eventListeners.end()) {
        m_queues[m_activeQueue].push_back(pEvent);
        return true;
    }
    else {
        return false;
    }
}

bool EventManager::VThreadSafeQueueEvent(const std::shared_ptr<IEventData>& pEvent) {
    m_realtimeEventQueue.push(pEvent);
    return true;
}

bool EventManager::VAbortEvent(const unsigned long& inType, bool allOfType) {

    bool success = false;
    auto findIt = m_eventListeners.find(inType);

    if (findIt != m_eventListeners.end()) {
        auto& eventQueue = m_queues[m_activeQueue];
        auto it = eventQueue.begin();
        while (it != eventQueue.end()) {
            // Removing an item from the queue will invalidate the iterator, so have it point to the next member.  All
            // work inside this loop will be done using thisIt.
            auto thisIt = it;
            ++it;

            if ((*thisIt)->VGetEventType() == inType) {
                eventQueue.erase(thisIt);
                success = true;
                if (!allOfType)
                    break;
            }
        }
    }

    return success;
}

bool EventManager::VUpdate(unsigned long maxMillis) {
    unsigned long currMs = GetTickCount();
    unsigned long maxMs = ((maxMillis == IEventManager::kINFINITE) ? (IEventManager::kINFINITE) : (currMs + maxMillis));

    // This section added to handle events from other threads.  Check out Chapter 20.
    std::shared_ptr<IEventData> pRealtimeEvent;
    while (!m_realtimeEventQueue.empty()) {
        pRealtimeEvent = m_realtimeEventQueue.front();
        m_realtimeEventQueue.pop();
        VQueueEvent(pRealtimeEvent);

        currMs = GetTickCount();
        if (maxMillis != IEventManager::kINFINITE) {
            if (currMs >= maxMs) {
            }
        }
    }

    // swap active queues and clear the new queue after the swap
    int queueToProcess = m_activeQueue;
    m_activeQueue = (m_activeQueue + 1) % EVENTMANAGER_NUM_QUEUES;
    m_queues[m_activeQueue].clear();

    // Process the queue
    while (!m_queues[queueToProcess].empty()) {
        // pop the front of the queue
        auto pEvent = m_queues[queueToProcess].front();
        m_queues[queueToProcess].pop_front();

        const unsigned long& eventType = pEvent->VGetEventType();

        // find all the delegate functions registered for this event
        auto findIt = m_eventListeners.find(eventType);
        if (findIt != m_eventListeners.end()) {
            const auto& eventListeners = findIt->second;

            // call each listener
            for (auto it = eventListeners.begin(); it != eventListeners.end(); ++it) {
                auto listener = (*it);
                listener(pEvent);
            }
        }

        // check to see if time ran out
        currMs = GetTickCount();
        if (maxMillis != IEventManager::kINFINITE && currMs >= maxMs) {
            break;
        }
    }

    // If we couldn't process all of the events, push the remaining events to the new active queue.
    // Note: To preserve sequencing, go back-to-front, inserting them at the head of the active queue
    bool queueFlushed = (m_queues[queueToProcess].empty());
    if (!queueFlushed) {
        while (!m_queues[queueToProcess].empty()) {
            auto pEvent = m_queues[queueToProcess].back();
            m_queues[queueToProcess].pop_back();
            m_queues[m_activeQueue].push_front(pEvent);
        }
    }

    return queueFlushed;
}