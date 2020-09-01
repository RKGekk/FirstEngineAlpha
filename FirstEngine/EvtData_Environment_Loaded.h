#pragma once

#include <memory>

#include "BaseEventData.h"

class EvtData_Environment_Loaded : public BaseEventData {
public:
	static const unsigned long sk_EventType = 0x8E2AD6E6;

	EvtData_Environment_Loaded(void) {}
	virtual const unsigned long& VGetEventType(void) const { return sk_EventType; }
	virtual std::shared_ptr<IEventData> VCopy(void) const {
		return std::shared_ptr<IEventData>(new EvtData_Environment_Loaded());
	}
	virtual const char* GetName(void) const { return "EvtData_Environment_Loaded"; }
};