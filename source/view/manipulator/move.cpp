#include "pixelboost/graphics/camera/camera.h"

#include "core/selection.h"
#include "view/entity/entity.h"
#include "view/manipulator/move.h"
#include "view/level.h"
#include "core.h"
#include "view.h"

using namespace pixeleditor;

MoveManipulator::MoveManipulator(pb::Scene* scene)
    : Manipulator(scene)
    , _Snap(0,0,0)
{
    
}

MoveManipulator::~MoveManipulator()
{
    
}

pb::Uid MoveManipulator::GetType() const
{
    return MoveManipulator::GetStaticType();
}

pb::Uid MoveManipulator::GetStaticType()
{
    return pb::TypeHash("MoveManipulator");
}

std::string MoveManipulator::GetName()
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

bool MoveManipulator::OnKeyDown(pb::KeyboardKey key, char character)
{
    return false;
}

bool MoveManipulator::OnKeyUp(pb::KeyboardKey key, char character)
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

glm::vec3 MoveManipulator::GetSnap() const
{
    return _Snap;
}

void MoveManipulator::SetSnap(glm::vec3 snap)
{
    _Snap = snap;
}
