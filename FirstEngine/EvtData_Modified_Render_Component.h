#pragma once

#include "BaseEventData.h"

// This event is sent out when a render component is changed
class EvtData_Modified_Render_Component : public BaseEventData {
    unsigned int m_id;

public:
    static const unsigned long sk_EventType = 0xe86c7c31;

    virtual const unsigned long& VGetEventType(void) const {
        return sk_EventType;
    }

    EvtData_Modified_Render_Component(void) {
        m_id = 0;
    }

    EvtData_Modified_Render_Component(unsigned int id)
        : m_id(id) {}

    virtual void VSerialize(std::ostrstream& out) const {
        out << m_id;
    }

    virtual void VDeserialize(std::istrstream& in) {
        in >> m_id;
    }

    virtual std::shared_ptr<IEventData> VCopy() const {
        return std::shared_ptr<IEventData>(new EvtData_Modified_Render_Component(m_id));
    }

    virtual const char* GetName(void) const {
        return "EvtData_Modified_Render_Component";
    }

    unsigned int GetActorId(void) const {
        return m_id;
    }
};