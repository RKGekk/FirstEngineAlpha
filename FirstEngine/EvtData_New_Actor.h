#pragma once

#include "BaseEventData.h"

// This event is sent out when an actor is *actually* created.
class EvtData_New_Actor : public BaseEventData {
	unsigned int m_actorId;
	unsigned int m_viewId;

public:
	static const unsigned long sk_EventType = 0xe86c7c31;

	EvtData_New_Actor() {
		m_actorId = 0;
		m_viewId = 0;
	}

	explicit EvtData_New_Actor(unsigned long actorId, unsigned long viewId = 0xffffffff) : m_actorId(actorId), m_viewId(viewId) {}

	virtual void VDeserialize(std::istrstream& in) {
		in >> m_actorId;
		in >> m_viewId;
	}

	virtual const unsigned long& VGetEventType(void) const {
		return sk_EventType;
	}

	virtual std::shared_ptr<IEventData> VCopy() const {
		return std::shared_ptr<IEventData>(new EvtData_New_Actor(m_actorId, m_viewId));
	}

	virtual void VSerialize(std::ostrstream& out) const {
		out << m_actorId << " ";
		out << m_viewId << " ";
	}


	virtual const char* GetName() const {
		return "EvtData_New_Actor";
	}

	const unsigned int GetActorId(void) const {
		return m_actorId;
	}

	unsigned int GetViewId(void) const {
		return m_viewId;
	}
};