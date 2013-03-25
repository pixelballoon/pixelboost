#pragma once

#include "pixelboost/logic/entity.h"

namespace pb
{
    class GuiRenderMessage;
}

class EditorUi : public pb::Entity
{
    PB_DECLARE_ENTITY
    
public:
    EditorUi(pb::Scene* scene, pb::Entity* parent, pb::DbEntity* creationEntity);
    ~EditorUi();
    
private:
    void OnGuiRender(const pb::Message& message);
    void OnUpdate(const pb::Message& message);
    
    void DoSceneViewPanel(const pb::GuiRenderMessage& guiRenderMessage);
    void DoPropertyPanel(const pb::GuiRenderMessage& guiRenderMessage);
    void DoContextPanel(const pb::GuiRenderMessage& guiRenderMessage);
    void DoViewports(const pb::GuiRenderMessage& guiRenderMessage);
};
