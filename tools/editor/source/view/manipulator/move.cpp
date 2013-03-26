#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/renderer/gui/controls.h"
#include "pixelboost/logic/message/graphics/gui.h"

#include "core/selection.h"
#include "view/entity/entity.h"
#include "view/manipulator/move.h"
#include "view/level.h"
#include "core.h"
#include "view.h"

PB_DEFINE_ENTITY(MoveManipulator)

MoveManipulator::MoveManipulator(pb::Scene* scene, pb::Entity* parent, pb::DbEntity* creationEntity)
    : Manipulator(scene, parent, creationEntity)
    , _Snap(0,0,0)
{
    
}

MoveManipulator::~MoveManipulator()
{
    
}

std::string MoveManipulator::GetManipulatorName()
{
    return "move";
}

char MoveManipulator::GetKey()
{
    return 'g';
}

bool MoveManipulator::OnMouseDown(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position)
{
    if (button == pb::kMouseButtonLeft)
    {
        Commit();
        View::Instance()->GetManipulatorManager()->SetActiveManipulator("select");
        return true;
    } else if (button == pb::kMouseButtonRight)
    {
        Reset();
        View::Instance()->GetManipulatorManager()->SetActiveManipulator("select");
        return true;
    }
    
    return false;
}

bool MoveManipulator::OnMouseUp(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position)
{
    return false;    
}

bool MoveManipulator::OnMouseMove(glm::vec2 position)
{
    if (!_Active)
    {
        _Start= position;
        _Active = true;
    }
    
    _End = position;
    
    glm::vec2 start = View::Instance()->GetLevelCamera()->ConvertScreenToWorld(_Start);
    glm::vec2 end = View::Instance()->GetLevelCamera()->ConvertScreenToWorld(_End);
    glm::vec2 transform = end-start;
    
    for (EntityList::iterator it = _EntityIds.begin(); it != _EntityIds.end(); ++it)
    {
        ViewEntity* entity = View::Instance()->GetLevel()->GetEntityById(*it);
        if (entity)
        {
            entity->ResetTransform();
            glm::vec3 position = entity->GetPosition() + glm::vec3(transform,0);
            if (_Snap.x != 0)
                position.x = position.x - glm::mod(position.x, _Snap.x);
            if (_Snap.y != 0)
                position.y = position.y - glm::mod(position.y, _Snap.y);
            if (_Snap.z != 0)
                position.z = position.z - glm::mod(position.z, _Snap.z);
            entity->Transform(position - entity->GetPosition());
        }
    }
    
    return false;
}

bool MoveManipulator::OnKeyDown(pb::KeyboardKey key, pb::ModifierKeys modifier, char character)
{
    return false;
}

bool MoveManipulator::OnKeyUp(pb::KeyboardKey key, pb::ModifierKeys modifier, char character)
{
    return false;
}

void MoveManipulator::Commit()
{
    for (EntityList::iterator it = _EntityIds.begin(); it != _EntityIds.end(); ++it)
    {
        ViewEntity* entity = View::Instance()->GetLevel()->GetEntityById(*it);
        if (entity)
            entity->CommitTransform();
    }
}

void MoveManipulator::Reset()
{
    for (EntityList::iterator it = _EntityIds.begin(); it != _EntityIds.end(); ++it)
    {
        ViewEntity* entity = View::Instance()->GetLevel()->GetEntityById(*it);
        if (entity)
            entity->ResetTransform();
    }
}

void MoveManipulator::OnSetActive()
{
    _Active = false;
    
    _EntityIds.clear();
    
    const Selection& selection = Core::Instance()->GetSelection();
    
    for (Selection::Entities::const_iterator entityIt = selection.GetSelection().begin(); entityIt != selection.GetSelection().end(); ++entityIt)
    {
        _EntityIds.push_back(entityIt->first);
    }
}

void MoveManipulator::OnSetInactive()
{
    
}

void MoveManipulator::DoGui(const pb::GuiRenderMessage& guiRenderMessage)
{
    pb::GuiControls::DoLabel(guiRenderMessage, PbGuiId(guiRenderMessage, 0), "Snap:");
    pb::GuiControls::BeginHorizontal(guiRenderMessage, PbGuiId(guiRenderMessage, 0));
    int i=0;
    for (float snap : {0.f, 0.25f, 0.5f, 1.f, 2.f, 5.f, 10.f})
    {
        char stringValue[32];
        snprintf(stringValue, 32, "%g", snap);
        if (pb::GuiControls::DoButton(guiRenderMessage, PbGuiId(guiRenderMessage, i), snap == 0.f ? "Off" : stringValue, {pb::GuiLayoutHint::ExpandWidth()}))
        {
            _Snap = glm::vec3(snap,snap,snap);
        }
        i++;
    }
    pb::GuiControls::EndHorizontal(guiRenderMessage);
}

glm::vec3 MoveManipulator::GetSnap() const
{
    return _Snap;
}

void MoveManipulator::SetSnap(glm::vec3 snap)
{
    _Snap = snap;
}
