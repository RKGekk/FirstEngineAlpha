#pragma once

#include <DirectXMath.h>
#include <memory>

#include "BaseEventData.h"

// Sent when actors are moved
class EvtData_Move_Actor : public BaseEventData {
    unsigned int m_id;
    DirectX::XMFLOAT4X4 m_matrix;

public:
    static const unsigned long sk_EventType = 0xeeaa0a40;

    virtual const unsigned long& VGetEventType() const {
        return sk_EventType;
    }

    EvtData_Move_Actor() {
        m_id = 0;
    }

    EvtData_Move_Actor(unsigned int id, const DirectX::XMFLOAT4X4& matrix)
        : m_id(id), m_matrix(matrix) {
        //
    }

    virtual void VSerialize(std::ostrstream& out) const {
        out << m_id << " ";
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                out << m_matrix.m[i][j] << " ";
            }
        }
    }

    virtual void VDeserialize(std::istrstream& in) {
        in >> m_id;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                in >> m_matrix.m[i][j];
            }
        }
    }

    virtual std::shared_ptr<IEventData> VCopy() const {
        return std::shared_ptr<IEventData>(new EvtData_Move_Actor(m_id, m_matrix));
    }

    virtual const char* GetName(void) const {
        return "EvtData_Move_Actor";
    }

    unsigned int GetId(void) const {
        return m_id;
    }

    const DirectX::XMFLOAT4X4& GetMatrix(void) const {
        return m_matrix;
    }
};