#include "glm/gtc/matrix_transform.hpp"

#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/renderer/primitive/primitiveRenderer.h"
#include "pixelboost/logic/component/graphics/rectangle.h"
#include "pixelboost/logic/component/transform.h"

#include "command/manager.h"
#include "view/entity/entity.h"
#include "view/manipulator/select.h"
#include "view/level.h"
#include "core.h"
#include "view.h"

PB_DEFINE_ENTITY(SelectManipulator)

SelectManipulator::SelectManipulator(pb::Scene* scene, pb::Entity* parent, pb::DbEntity* creationEntity)
    : Manipulator(scene, parent, creationEntity)
    , _Active(false)
    , _BoundsComponent(0)
{
    CreateComponent<pb::TransformComponent>();
}

SelectManipulator::~SelectManipulator()
{
    
}

std::string SelectManipulator::GetManipulatorName()
{
    return "select";
}

char SelectManipulator::GetKey()
{
    return 'q';
}

void SelectManipulator::Render(int layer)
{
    if (_Active && _Mode == kModeSelect)
    {
        if (!_BoundsComponent)
        {
            _BoundsComponent = CreateComponent<pb::RectangleComponent>();
            _BoundsComponent->SetColor(glm::vec4(0.0, 0.0, 0.1, 0.1));
            _BoundsComponent->SetSolid(true);
            _BoundsComponent->SetLayer(2);
        }
        
        glm::vec3 start = View::Instance()->GetActiveViewport()->ConvertScreenToWorld(_MouseStart);
        glm::vec3 end = View::Instance()->GetActiveViewport()->ConvertScreenToWorld(_MouseEnd);
        glm::vec3 position = (start+end)/2.f;
        glm::vec3 size = end-start;
        
        glm::mat4x4 transform = glm::translate(glm::mat4x4(), position);
        
        _BoundsComponent->SetSize(glm::vec2(size.x, size.y));
        _BoundsComponent->SetLocalTransform(transform);
    } else {
        if (_BoundsComponent)
        {
            DestroyComponent(_BoundsComponent);
            _BoundsComponent = 0;
        }
    }
}

bool SelectManipulator::OnMouseDown(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position)
{
    if (button == pb::kMouseButtonLeft)
    {
        _MouseStart = _MouseEnd = position;
        
        if (modifierKeys == pb::kModifierKeyNone || modifierKeys == pb::kModifierKeyShift || modifierKeys == pb::kModifierKeyCtrl)
        {
            _Active = true;
            _Mode = kModeSelect;
        } else if (modifierKeys == pb::kModifierKeyAlt)
        {
            _Active = true;
            _Mode = kModeMove;
        } else if (modifierKeys == (pb::kModifierKeyAlt | pb::kModifierKeyShift))
        {
            if (View::Instance()->GetActiveViewport()->GetType() == Viewport::kViewportTypePerspective)
            {
                _Active = true;
                _Mode = kModeRotate;
            }
        }
        
        return true;
    }

    return false;
}

bool SelectManipulator::OnMouseUp(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position)
{
    if (_Active && button == pb::kMouseButtonLeft)
    {
        _Active = false;
        _MouseEnd = position;
        
        if (_Mode == kModeSelect)
        {
            glm::vec3 start = View::Instance()->GetActiveViewport()->ConvertScreenToWorld(_MouseStart);
            glm::vec3 end = View::Instance()->GetActiveViewport()->ConvertScreenToWorld(_MouseEnd);
            glm::vec2 min(glm::min(start.x, end.x), glm::min(start.y, end.y));
            glm::vec2 max(glm::max(start.x, end.x), glm::max(start.y, end.y));
            
            Level* level = View::Instance()->GetLevel();
            
            std::string entityIds;
            
            pb::BoundingBox selectionBounds(glm::vec3(min, 0), glm::vec3(max, 0));

            bool singleSelection = min == max;
            
            Level::EntityList entities = level->GetEntitiesInBounds(selectionBounds);
            
            for (Level::EntityList::iterator it = entities.begin(); it != entities.end(); ++it)
            {
                char id[32];
                sprintf(id, "%d", (*it)->GetEntityUid());

                entityIds += std::string((entityIds.length() == 0 ? "" : ",")) + id;

                if (singleSelection)
                {
                    break;
                }
            }

            if (entityIds.length())
            {
                Core::Instance()->GetCommandManager()->Exec("select", "-u " + entityIds);
            } else {
                Core::Instance()->GetCommandManager()->Exec("select", "-c");
            }

            return true;
        }
        
        if (_Mode == kModeMove)
        {
            glm::vec3 start = View::Instance()->GetActiveViewport()->ConvertScreenToWorld(_MouseStart);
            glm::vec3 end = View::Instance()->GetActiveViewport()->ConvertScreenToWorld(_MouseEnd);
            
            View::Instance()->GetActiveViewport()->GetCamera()->Position += glm::vec3(end-start);
        }
    }
    
    return false;
}

bool SelectManipulator::OnMouseMove(glm::vec2 position)
{
    if (_Active)
    {
        _MouseEnd = position;
        
        if (_Mode == kModeMove)
        {
            glm::vec3 start = View::Instance()->GetActiveViewport()->ConvertScreenToWorld(_MouseStart);
            glm::vec3 end = View::Instance()->GetActiveViewport()->ConvertScreenToWorld(_MouseEnd);
            _MouseStart = position;
            
            View::Instance()->GetActiveViewport()->GetCamera()->Position += glm::vec3(start-end);
        }
        
        if (_Mode == kModeRotate)
        {
            View::Instance()->GetActiveViewport()->GetCamera()->Rotation -= glm::vec3(_MouseStart.y-_MouseEnd.y, _MouseStart.x-_MouseEnd.x, 0)/2.f;
            _MouseStart = position;
        }
    }
    
    return false;
}

bool SelectManipulator::OnKeyDown(pb::KeyboardKey key, pb::ModifierKeys modifier, char character)
{
    return false;
}

bool SelectManipulator::OnKeyUp(pb::KeyboardKey key, pb::ModifierKeys modifier, char character)
{
    return false;
}

void SelectManipulator::OnSetActive()
{
    
}

void SelectManipulator::OnSetInactive()
{
    
}
