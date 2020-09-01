#pragma once

#include <DirectXMath.h>
#include <memory>

#include "BaseEventData.h"

class EvtData_Request_New_Actor : public BaseEventData {
	std::string			m_actorResource;
	bool				m_hasInitialTransform;
	DirectX::XMFLOAT4X4	m_initialTransform;
	unsigned int		m_serverActorId;

public:
	static const unsigned long sk_EventType = 0x40378c64;

	EvtData_Request_New_Actor() {
		m_actorResource = "";
		m_hasInitialTransform = false;
		DirectX::XMStoreFloat4x4(&m_initialTransform, DirectX::XMMatrixIdentity());
		m_serverActorId = -1;
	}

	explicit EvtData_Request_New_Actor(const std::string& actorResource, const DirectX::XMFLOAT4X4* initialTransform = NULL, const unsigned int serverActorId = 0) {
		m_actorResource = actorResource;
		if (initialTransform) {
			m_hasInitialTransform = true;
			m_initialTransform = *initialTransform;
		}
		else
			m_hasInitialTransform = false;

		m_serverActorId = serverActorId;
	}

	virtual const unsigned long& VGetEventType() const {
		return sk_EventType;
	}

	virtual void VDeserialize(std::istrstream& in) {
		in >> m_actorResource;
		in >> m_hasInitialTransform;
		if (m_hasInitialTransform) {
			for (int i = 0; i < 4; ++i) {
				for (int j = 0; j < 4; ++j) {
					in >> m_initialTransform.m[i][j];
				}
			}
		}
		in >> m_serverActorId;
	}

	virtual std::shared_ptr<IEventData> VCopy() const {
		return std::shared_ptr<IEventData>(new EvtData_Request_New_Actor(m_actorResource, (m_hasInitialTransform) ? &m_initialTransform : NULL, m_serverActorId));
	}

	virtual void VSerialize(std::ostrstream& out) const {
		out << m_actorResource << " ";
		out << m_hasInitialTransform << " ";
		if (m_hasInitialTransform) {
			for (int i = 0; i < 4; ++i) {
				for (int j = 0; j < 4; ++j) {
					out << m_initialTransform.m[i][j] << " ";
				}
			}
		}
		out << m_serverActorId << " ";
	}

	virtual const char* GetName(void) const { return "EvtData_Request_New_Actor"; }

	const std::string& GetActorResource(void) const { return m_actorResource; }
	const DirectX::XMFLOAT4X4* GetInitialTransform(void) const { return (m_hasInitialTransform) ? &m_initialTransform : NULL; }
	const unsigned int GetServerActorId(void) const { return m_serverActorId; }
};