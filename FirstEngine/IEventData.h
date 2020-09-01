#pragma once

#include <strstream>
#include <memory>

class IEventData {
public:
	virtual ~IEventData(void) {}
	virtual const unsigned long& VGetEventType(void) const = 0;
	virtual float GetTimeStamp(void) const = 0;
	virtual void VSerialize(std::ostrstream& out) const = 0;
	virtual void VDeserialize(std::istrstream& in) = 0;
	virtual std::shared_ptr<IEventData> VCopy(void) const = 0;
	virtual const char* GetName(void) const = 0;

	//GCC_MEMORY_WATCHER_DECLARATION();
};