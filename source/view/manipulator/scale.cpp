#include "pixelboost/graphics/camera/camera.h"

#include "core/selection.h"
#include "view/entity/entity.h"
#include "view/manipulator/scale.h"
#include "view/level.h"
#include "core.h"
#include "view.h"

using namespace pixeleditor;

ScaleManipulator::ScaleManipulator(pb::Scene* scene)
    : Manipulator(scene)
{
    
}

ScaleManipulator::~ScaleManipulator()
{
    
}

pb::Uid ScaleManipulator::GetType() const
{
    return ScaleManipulator::GetStaticType();
}

pb::Uid ScaleManipulator::GetStaticType()
{
    return pb::TypeHash("ScaleManipulator");
}

std::string ScaleManipulator::GetName()
{
    return "scale";
}

char ScaleManipulator::GetKey()
{
    return 's';
}

bool ScaleManipulator::OnMouseDown(pb::MouseButton button, pb::ModifierKeys modifierKeys,  glm::vec2 position)
{
    if (button == pb::kMouseButtonLeft)
    {
        Commit();
        View::Instance()->GetManipulatorManager()->SetActiveManipulator("select");
        return true;
    }
    
    if (button == pb::kMouseButtonRight)
    {
        Reset();
        View::Instance()->GetManipulatorManager()->SetActiveManipulator("select");
        return true;
    }
    
    return false;
}

bool ScaleManipulator::OnMouseUp(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position)
{
    return false;    
}

bool ScaleManipulator::OnMouseMove(glm::vec2 position)
{
    if (!_Active)
    {
        _Start= position;
        _Active = true;
    }
    
    _End = position;
    
    glm::vec2 start = View::Instance()->GetLevelCamera()->ConvertScreenToWorld(_Start);
    glm::vec2 end = View::Instance()->GetLevelCamera()->ConvertScreenToWorld(_End);
    float startLength = glm::length(start-glm::vec2(_Center.x, _Center.y));
    float currentLength = glm::length(end-glm::vec2(_Center.x, _Center.y));
    
    if (startLength > 0)
    {
        float scale = currentLength / startLength;
        
        for (EntityList::iterator it = _EntityIds.begin(); it != _EntityIds.end(); ++it)
        {
            ViewEntity* entity = View::Instance()->GetLevel()->GetEntityById(*it);
            if (entity)
            {
                entity->ResetTransform();
                entity->Transform(glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(scale,scale,scale));
            }
        }
    }
    
    return false;
}

bool ScaleManipulator::OnKeyDown(pb::KeyboardKey key, char character)
{
    return false;
}

bool ScaleManipulator::OnKeyUp(pb::KeyboardKey key, char character)
{
    return false;
}

void ScaleManipulator::Commit()
{
    for (EntityList::iterator it = _EntityIds.begin(); it != _EntityIds.end(); ++it)
    {
        ViewEntity* entity = View::Instance()->GetLevel()->GetEntityById(*it);
        if (entity)
            entity->CommitTransform();
    }
}

void ScaleManipulator::Reset()
{
    for (EntityList::iterator it = _EntityIds.begin(); it != _EntityIds.end(); ++it)
    {
        ViewEntity* entity = View::Instance()->GetLevel()->GetEntityById(*it);
        if (entity)
            entity->ResetTransform();
    }
}

void ScaleManipulator::OnSetActive()
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

void ScaleManipulator::OnSetInactive()
{
    
}
