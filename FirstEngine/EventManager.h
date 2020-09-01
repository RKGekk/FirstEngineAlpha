#pragma once

#include <list>
#include <queue>
#include <memory>

#include "FastDelegate.h"

#include "IEventManager.h"

const unsigned int EVENTMANAGER_NUM_QUEUES = 2;

class EventManager : public IEventManager {

    std::map<unsigned long, std::list<fastdelegate::FastDelegate1<std::shared_ptr<IEventData>>>> m_eventListeners;
    std::list<std::shared_ptr<IEventData>> m_queues[EVENTMANAGER_NUM_QUEUES];
    int m_activeQueue;  // index of actively processing queue; events enque to the opposing queue

    std::queue<std::shared_ptr<IEventData>> m_realtimeEventQueue;

public:
    explicit EventManager(const char* pName, bool setAsGlobal);
    virtual ~EventManager(void);

    virtual bool VAddListener(const fastdelegate::FastDelegate1<std::shared_ptr<IEventData>>& eventDelegate, const unsigned long& type);
    virtual bool VRemoveListener(const fastdelegate::FastDelegate1<std::shared_ptr<IEventData>>& eventDelegate, const unsigned long& type);

    virtual bool VTriggerEvent(const std::shared_ptr<IEventData>& pEvent) const;
    virtual bool VQueueEvent(const std::shared_ptr<IEventData>& pEvent);
    virtual bool VThreadSafeQueueEvent(const std::shared_ptr<IEventData>& pEvent);
    virtual bool VAbortEvent(const unsigned long& inType, bool allOfType);

    virtual bool VUpdate(unsigned long maxMillis = kINFINITE);
};