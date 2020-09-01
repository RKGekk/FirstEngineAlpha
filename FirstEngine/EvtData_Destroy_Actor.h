#pragma once

#include <memory>

#include "IEventData.h"
#include "BaseEventData.h"

// Sent when actors are destroyed	
class EvtData_Destroy_Actor : public BaseEventData {
    unsigned int m_id;

public:
    static const unsigned long sk_EventType = 0x77dd2b3a;

    explicit EvtData_Destroy_Actor(unsigned int id = 0)
        : m_id(id) {}

    virtual const unsigned long& VGetEventType(void) const {
        return sk_EventType;
    }

    virtual std::shared_ptr<IEventData> VCopy(void) const {
        return std::shared_ptr<IEventData>(new EvtData_Destroy_Actor(m_id));
    }

    virtual void VSerialize(std::ostrstream& out) const {
        out << m_id;
    }

    virtual void VDeserialize(std::istrstream& in) {
        in >> m_id;
    }

    virtual const char* GetName(void) const {
        return "EvtData_Destroy_Actor";
    }

    unsigned int GetId(void) const { return m_id; }
};