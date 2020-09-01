#pragma once

#include <memory>

#include "BaseEventData.h"
#include "SceneNode.h"

// This event is sent out when an actor is *actually* created.
class EvtData_New_Render_Component : public BaseEventData {
    unsigned int m_actorId;
    std::shared_ptr<SceneNode> m_pSceneNode;

public:
    static const unsigned long sk_EventType = 0xaf4aff75;

    EvtData_New_Render_Component() {
        m_actorId = 0;
    }

    explicit EvtData_New_Render_Component(unsigned int actorId, std::shared_ptr<SceneNode> pSceneNode) : m_actorId(actorId), m_pSceneNode(pSceneNode) {}

    virtual void VSerialize(std::ostrstream& out) const {}
    virtual void VDeserialize(std::istrstream& in) {}

    virtual const unsigned long& VGetEventType(void) const {
        return sk_EventType;
    }

    virtual std::shared_ptr<IEventData> VCopy(void) const {
        return std::shared_ptr<IEventData>(new EvtData_New_Render_Component(m_actorId, m_pSceneNode));
    }

    virtual const char* GetName(void) const {
        return "EvtData_New_Render_Component";
    }

    const unsigned int GetActorId(void) const {
        return m_actorId;
    }

    std::shared_ptr<SceneNode> GetSceneNode(void) const {
        return m_pSceneNode;
    }
};