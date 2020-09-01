#pragma once

#include "IEventData.h"

class BaseEventData : public IEventData {
	const float m_timeStamp;

public:
	explicit BaseEventData(const float timeStamp = 0.0f) : m_timeStamp(timeStamp) {}

	// Returns the type of the event
	virtual const unsigned long& VGetEventType(void) const = 0;

	float GetTimeStamp(void) const { return m_timeStamp; }

	// Serializing for network input / output
	virtual void VSerialize(std::ostrstream& out) const {}
	virtual void VDeserialize(std::istrstream& in) {}
};