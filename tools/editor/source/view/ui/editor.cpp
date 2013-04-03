#include "pixelboost/debug/log.h"
#include "pixelboost/graphics/renderer/gui/controls.h"
#include "pixelboost/logic/component/graphics/gui.h"
#include "pixelboost/logic/component/transform.h"
#include "pixelboost/logic/message/graphics/gui.h"
#include "pixelboost/logic/message/update.h"

#include "command/manager.h"
#include "core/selection.h"
#include "project/entity.h"
#include "project/project.h"
#include "project/record.h"
#include "project/schema.h"
#include "view/entity/entity.h"
#include "view/manipulator/create.h"
#include "view/manipulator/manipulator.h"
#include "view/ui/editor.h"
#include "view/ui/property.h"
#include "view/level.h"
#include "core.h"
#include "view.h"

PB_DEFINE_ENTITY(EditorUi)

void DoViewport(const pb::GuiRenderMessage& message, pb::GuiId guiId, const std::vector<pb::GuiLayoutHint>& hints)
{
    if (message.GetEventType() == pb::GuiRenderMessage::kEventTypeLayout)
    {
        message.GetGuiSystem()->AddLayout("viewport", guiId, hints, glm::vec2(100.f, 100.f));
    }
    
    if (message.GetEventType() == pb::GuiRenderMessage::kEventTypeRender)
    {
        // TODO : Set viewport size
        //auto layout = message.GetGuiSystem()->GetLayout(guiId);
    }
}

EditorUi::EditorUi(pb::Scene* scene, pb::Entity* parent, pb::DbEntity* creationEntity)
    : pb::Entity(scene, parent, creationEntity)
{
    _PropertyUi = new PropertyUi();
    
    CreateComponent<pb::TransformComponent>();
    CreateComponent<pb::GuiComponent>()->SetGroup(0b10);
    
    RegisterMessageHandler<pb::GuiRenderMessage>(pb::MessageHandler(this, &EditorUi::OnGuiRender));
    RegisterMessageHandler<pb::UpdateMessage>(pb::MessageHandler(this, &EditorUi::OnUpdate));
}

EditorUi::~EditorUi()
{
    delete _PropertyUi;
}

void EditorUi::OnGuiRender(const pb::Message& message)
{
    auto guiRenderMessage = message.As<pb::GuiRenderMessage>();
    
    pb::GuiControls::BeginHorizontal(guiRenderMessage, PbGuiId(guiRenderMessage, 0), {pb::GuiLayoutHint::ExpandWidth(), pb::GuiLayoutHint::ExpandHeight()});
    
    DoSceneViewPanel(guiRenderMessage);

    pb::GuiControls::BeginVertical(guiRenderMessage, PbGuiId(guiRenderMessage, 0), {pb::GuiLayoutHint::ExpandWidth(), pb::GuiLayoutHint::ExpandHeight()});
    
    auto fileArea = pb::GuiControls::BeginArea(guiRenderMessage, PbGuiId(guiRenderMessage, 0), {pb::GuiLayoutHint::Height(20.f), pb::GuiLayoutHint::ExpandWidth()});
    pb::GuiControls::BeginHorizontal(guiRenderMessage, PbGuiId(guiRenderMessage, 0));
    if (pb::GuiControls::DoButton(guiRenderMessage, PbGuiId(guiRenderMessage, 0), "Save", {pb::GuiLayoutHint::Width(80)}))
    {
        Core::Instance()->GetCommandManager()->Exec("save");
    }
    if (pb::GuiControls::DoButton(guiRenderMessage, PbGuiId(guiRenderMessage, 0), "Export", {pb::GuiLayoutHint::Width(80)}))
    {
        Core::Instance()->GetCommandManager()->Exec("export");
    }
    pb::GuiControls::EndHorizontal(guiRenderMessage);
    pb::GuiControls::EndArea(guiRenderMessage, fileArea);
    
    DoViewports(guiRenderMessage);
    
    DoPropertyPanel(guiRenderMessage);

    pb::GuiControls::EndVertical(guiRenderMessage);
    
    DoContextPanel(guiRenderMessage);
    
    pb::GuiControls::EndHorizontal(guiRenderMessage);
}

void EditorUi::OnUpdate(const pb::Message& message)
{
    glm::vec3 position = glm::vec3(-pb::GraphicsDevice::Instance()->GetDisplayResolution()/(pb::GraphicsDevice::Instance()->GetDisplayDensity()*2.f), 0.f);
    position.y = -position.y;
    GetComponent<pb::TransformComponent>()->SetPosition(position);
    GetComponent<pb::GuiComponent>()->SetSize(pb::GraphicsDevice::Instance()->GetDisplayResolution());
}

void EditorUi::DoSceneViewPanel(const pb::GuiRenderMessage& guiRenderMessage)
{
    auto sceneArea = pb::GuiControls::BeginScrollArea(guiRenderMessage, PbGuiId(guiRenderMessage, 0), {pb::GuiLayoutHint::Width(300), pb::GuiLayoutHint::ExpandHeight()});
    
    auto selectedRecord = View::Instance()->GetRecord();
    
    if (selectedRecord)
    {
        if (pb::GuiControls::DoButton(guiRenderMessage, PbGuiId(guiRenderMessage, 0), "<-- Back"))
        {
            View::Instance()->SetRecord(0);
        }
        
        pb::GuiControls::DoLabel(guiRenderMessage, PbGuiId(guiRenderMessage, 0), "Record Name:");
        std::pair<bool, std::string> recordName = pb::GuiControls::DoTextBox(guiRenderMessage, PbGuiId(guiRenderMessage, 0), selectedRecord->GetName());
        if (recordName.first)
        {
            selectedRecord->SetName(recordName.second);
        }
        
        if (selectedRecord->GetType()->GetAttribute("HasLevel"))
        {
            pb::GuiControls::DoLabel(guiRenderMessage, PbGuiId(guiRenderMessage, 0), "Entities:");
            
            std::vector<ProjectEntity*> entities;
            for (auto entity : selectedRecord->GetEntities())
            {
                entities.push_back(entity.second);
            }
            
            std::sort(entities.begin(), entities.end(), [](ProjectEntity* a, ProjectEntity*b) { return a->GetName() < b->GetName(); });
            
            for (const auto& entity : entities)
            {
                if (pb::GuiControls::DoButton(guiRenderMessage, PbGuiId(guiRenderMessage, entity->GetUid()), entity->GetName()))
                {
                    char id[32];
                    snprintf(id, 32, "-u %d", entity->GetUid());
                    Core::Instance()->GetCommandManager()->Exec("select", id);
                }
            }
        }
    } else
    {
        pb::GuiControls::DoLabel(guiRenderMessage, PbGuiId(guiRenderMessage, 0), "Controls:");
        
        int selectedPage = pb::GuiControls::DoPageSelector(guiRenderMessage, PbGuiId(guiRenderMessage, 0), {"Create Record", "Open Record"});
        
        switch (selectedPage)
        {
            case 0:
                DoCreateRecordPage(guiRenderMessage);
                break;
                
            case 1:
                DoOpenRecordPage(guiRenderMessage);
                break;
        }
    }
    
    pb::GuiControls::EndScrollArea(guiRenderMessage, sceneArea);
}

void EditorUi::DoPropertyPanel(const pb::GuiRenderMessage& guiRenderMessage)
{
    auto selectedRecord = View::Instance()->GetRecord();
    if (selectedRecord)
    {
        std::vector<pb::GuiLayoutHint> hints = {pb::GuiLayoutHint::ExpandWidth()};
        
        if (selectedRecord->GetType()->GetAttribute("HasLevel"))
        {
            hints.push_back(pb::GuiLayoutHint::Height(300.f));
        } else {
            hints.push_back(pb::GuiLayoutHint::ExpandHeight());
        }
        
        auto propertyArea = pb::GuiControls::BeginScrollArea(guiRenderMessage, PbGuiId(guiRenderMessage, 0), hints);
        
        auto selection = Core::Instance()->GetSelection();
        
        if (!selectedRecord->GetType()->GetAttribute("HasLevel") || !selection.GetSelection().size())
        {
            _PropertyUi->DoProperties(guiRenderMessage, selectedRecord);
        } else {
            Selection::Entities::const_reverse_iterator entityIt = selection.GetSelection().rbegin();
            
            if (entityIt == selection.GetSelection().rend())
            {
                _PropertyUi->DoProperties(guiRenderMessage, selectedRecord);
            } else
            {
                ViewEntity* entity = View::Instance()->GetLevel()->GetEntityById(entityIt->first);
                
                if (entity)
                {
                    _PropertyUi->DoProperties(guiRenderMessage, entity->GetEntity());
                }
            }
        }
        
        pb::GuiControls::EndScrollArea(guiRenderMessage, propertyArea);
    }
}

void EditorUi::DoContextPanel(const pb::GuiRenderMessage& guiRenderMessage)
{
    auto selectedRecord = View::Instance()->GetRecord();
    if (selectedRecord && selectedRecord->GetType()->GetAttribute("HasLevel"))
    {
        auto controlArea = pb::GuiControls::BeginScrollArea(guiRenderMessage, PbGuiId(guiRenderMessage, 0), {pb::GuiLayoutHint::Width(300), pb::GuiLayoutHint::ExpandHeight()});
        
        if (View::Instance()->GetManipulatorManager()->GetActiveManipulator()->GetManipulatorName() == "select")
        {
            DoContextCreate(guiRenderMessage);
        } else {
            DoContextManipulator(guiRenderMessage);
        }
        
        pb::GuiControls::EndScrollArea(guiRenderMessage, controlArea);
    }
}

void EditorUi::DoViewports(const pb::GuiRenderMessage& guiRenderMessage)
{
    auto selectedRecord = View::Instance()->GetRecord();
    if (selectedRecord && selectedRecord->GetType()->GetAttribute("HasLevel"))
    {
        DoViewport(guiRenderMessage, PbGuiId(guiRenderMessage, 0), {pb::GuiLayoutHint::ExpandWidth(), pb::GuiLayoutHint::ExpandHeight()});
    }
}

void EditorUi::DoContextCreate(const pb::GuiRenderMessage& guiRenderMessage)
{
    pb::GuiControls::DoLabel(guiRenderMessage, PbGuiId(guiRenderMessage, 0), "Create Entity:");
    
    Schema* schema = Core::Instance()->GetProject()->GetSchema();
    
    for (const auto& entity : schema->GetEntities())
    {
        if (pb::GuiControls::DoButton(guiRenderMessage, PbGuiId(guiRenderMessage, pb::TypeHash(entity.first.c_str())), entity.first))
        {
            CreateManipulator* createManipulator = static_cast<CreateManipulator*>(View::Instance()->GetManipulatorManager()->SetActiveManipulator("create"));
            
            createManipulator->SetEntityType(entity.first);
        }
    }
}

void EditorUi::DoContextManipulator(const pb::GuiRenderMessage& guiRenderMessage)
{
    View::Instance()->GetManipulatorManager()->GetActiveManipulator()->DoGui(guiRenderMessage);
}

void EditorUi::DoCreateRecordPage(const pb::GuiRenderMessage& guiRenderMessage)
{
    pb::GuiControls::DoLabel(guiRenderMessage, PbGuiId(guiRenderMessage, 0), "Name:");
    
    auto recordName = pb::GuiControls::DoTextBox(guiRenderMessage, PbGuiId(guiRenderMessage, 0), _RecordName);
    if (recordName.first)
    {
        _RecordName = recordName.second;
    }
    
    pb::GuiControls::DoLabel(guiRenderMessage, PbGuiId(guiRenderMessage, 0), "Type:");
    
    std::vector<std::string> typeOptions;
    for (const auto& record : Core::Instance()->GetProject()->GetSchema()->GetRecords())
    {
        typeOptions.push_back(record.second->GetName());
    }
    
    int selectedType = pb::GuiControls::DoCombo(guiRenderMessage, PbGuiId(guiRenderMessage, 0), _RecordType, typeOptions);
    if (selectedType >= 0)
    {
        _RecordType = typeOptions[selectedType];
    }
    
    pb::GuiControls::DoSpacer(guiRenderMessage, PbGuiId(guiRenderMessage, 0), glm::vec2(0,20));
    
    if (pb::GuiControls::DoButton(guiRenderMessage, PbGuiId(guiRenderMessage, 0), "Create Record"))
    {
        if (_RecordName.length() && _RecordType.length() && Core::Instance()->GetProject()->AddRecord(_RecordName, _RecordType))
        {
            View::Instance()->SetRecord(Core::Instance()->GetProject()->GetRecordByName(_RecordName));
        }
    }
}

void EditorUi::DoOpenRecordPage(const pb::GuiRenderMessage& guiRenderMessage)
{
    pb::GuiControls::DoLabel(guiRenderMessage, PbGuiId(guiRenderMessage, 0), "Filter:");
    
    auto filterText = pb::GuiControls::DoTextBox(guiRenderMessage, PbGuiId(guiRenderMessage, 0), _RecordFilter);
    if (filterText.first)
    {
        _RecordFilter = filterText.second;
    }
    
    pb::GuiControls::DoLabel(guiRenderMessage, PbGuiId(guiRenderMessage, 0), "Records:");
    
    std::vector<ProjectRecord*> records;
    for (auto record : Core::Instance()->GetProject()->GetRecords())
    {
        if (_RecordFilter.length() == 0 || record.second->GetName().find(_RecordFilter) != std::string::npos)
        {
            records.push_back(record.second);
        }
    }
    
    std::sort(records.begin(), records.end(), [](ProjectRecord* a, ProjectRecord*b) { return a->GetName() < b->GetName(); });
    
    for (const auto& record : records)
    {
        if (pb::GuiControls::DoButton(guiRenderMessage, PbGuiId(guiRenderMessage, record->GetUid()), record->GetName()))
        {
            View::Instance()->SetRecord(record);
        }
    }
}
