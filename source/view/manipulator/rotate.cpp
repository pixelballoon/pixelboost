#include "pixelboost/graphics/camera/camera.h"

#include "core/selection.h"
#include "view/entity/entity.h"
#include "view/manipulator/rotate.h"
#include "view/level.h"
#include "core.h"
#include "view.h"

using namespace pixeleditor;

RotateManipulator::RotateManipulator(pb::Scene* scene)
    : Manipulator(scene)
{
    
}

RotateManipulator::~RotateManipulator()
{
    
}

std::string RotateManipulator::GetName()
{
    return "rotate";
}

char RotateManipulator::GetKey()
{
    return 'r';
}

bool RotateManipulator::OnMouseDown(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position)
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

bool RotateManipulator::OnMouseUp(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position)
{
    return false;    
}

bool RotateManipulator::OnMouseMove(glm::vec2 position)
{
    if (!_Active)
    {
        _Start= position;
        _Active = true;
    }
    
    _End = position;
    
    glm::vec2 start = View::Instance()->GetLevelCamera()->ConvertScreenToWorld(_Start);
    glm::vec2 end = View::Instance()->GetLevelCamera()->ConvertScreenToWorld(_End);
    
    glm::vec2 startLine = start-glm::vec2(_Center.x, _Center.y);
    glm::vec2 endLine = end-glm::vec2(_Center.x, _Center.y);

    float startAngle = glm::atan(startLine.y, startLine.x);
    float endAngle = glm::atan(endLine.y, endLine.x);
    
    float angle = glm::degrees(endAngle-startAngle);
    
    for (EntityList::iterator it = _EntityIds.begin(); it != _EntityIds.end(); ++it)
    {
        ViewEntity* entity = View::Instance()->GetLevel()->GetEntityById(*it);
        if (entity)
        {
            entity->ResetTransform();
            glm::vec3 rotation = entity->GetRotation() + glm::vec3(0,0,angle);
            if (_Snap.x != 0)
                rotation.x = rotation.x - glm::mod(rotation.x, _Snap.x);
            if (_Snap.y != 0)
                rotation.y = rotation.y - glm::mod(rotation.y, _Snap.y);
            if (_Snap.z != 0)
                rotation.z = rotation.z - glm::mod(rotation.z, _Snap.z);
            entity->Transform(glm::vec3(0,0,0), rotation-entity->GetRotation());
        }
    }
    
    return false;
}

bool RotateManipulator::OnKeyDown(pb::KeyboardKey key, char character)
{
    return false;
}

bool RotateManipulator::OnKeyUp(pb::KeyboardKey key, char character)
{
    return false;
}

void RotateManipulator::Commit()
{
    for (EntityList::iterator it = _EntityIds.begin(); it != _EntityIds.end(); ++it)
    {
        ViewEntity* entity = View::Instance()->GetLevel()->GetEntityById(*it);
        if (entity)
            entity->CommitTransform();
    }
}

void RotateManipulator::Reset()
{
    for (EntityList::iterator it = _EntityIds.begin(); it != _EntityIds.end(); ++it)
    {
        ViewEntity* entity = View::Instance()->GetLevel()->GetEntityById(*it);
        if (entity)
            entity->ResetTransform();
    }
}

void RotateManipulator::OnSetActive()
{
    _Active = false;
    
    _EntityIds.clear();
    
    const Selection& selection = Core::Instance()->GetSelection();
    
    int entityCount = 0;
    _Center = glm::vec3(0,0,0);
    
    for (Selection::Entities::const_iterator entityIt = selection.GetSelection().begin(); entityIt != selection.GetSelection().end(); ++entityIt)
    {
        _EntityIds.push_back(entityIt->first);
        
        ViewEntity* entity = View::Instance()->GetLevel()->GetEntityById(entityIt->first);
        if (entity)
        {
            entityCount++;
            _Center += entity->GetPosition();
        }
    }
    
    if (entityCount)
        _Center = _Center / (float)entityCount;
}

void RotateManipulator::OnSetInactive()
{
    
}

glm::vec3 RotateManipulator::GetSnap() const
{
    return _Snap;
}

void RotateManipulator::SetSnap(glm::vec3 snap)
{
    _Snap = snap;
}
