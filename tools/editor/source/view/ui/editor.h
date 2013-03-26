#pragma once

#include "pixelboost/logic/entity.h"

namespace pb
{
    class GuiRenderMessage;
}

class PropertyUi;

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
    
    void DoContextCreate(const pb::GuiRenderMessage& guiRenderMessage);
    void DoContextManipulator(const pb::GuiRenderMessage& guiRenderMessage);
    
    void DoCreateRecordPage(const pb::GuiRenderMessage& guiRenderMessage);
    void DoOpenRecordPage(const pb::GuiRenderMessage& guiRenderMessage);

    std::string _RecordFilter;
    std::string _RecordName;
    std::string _RecordType;
    
    PropertyUi* _PropertyUi;
};
