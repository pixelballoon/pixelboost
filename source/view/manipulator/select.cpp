#include "pixelboost/graphics/camera/camera.h"
#include "pixelboost/graphics/render/primitive/primitiveRenderer.h"

#include "command/manager.h"
#include "view/entity/entity.h"
#include "view/manipulator/select.h"
#include "view/level.h"
#include "core.h"
#include "view.h"

using namespace pixeleditor;

SelectManipulator::SelectManipulator()
    : _Active(false)
{
    
}

SelectManipulator::~SelectManipulator()
{
    
}

std::string SelectManipulator::GetName()
{
    return "select";
}

char SelectManipulator::GetKey()
{
    return 'q';
}

void SelectManipulator::Render(pb::RenderLayer* layer)
{
    if (_Active)
    {
        pb::OrthographicCamera* camera = View::Instance()->GetLevelCamera();
        
        glm::vec2 start = camera->ConvertScreenToWorld(_SelectStart);
        glm::vec2 end = camera->ConvertScreenToWorld(_SelectEnd);
        glm::vec2 position = (start+end)/2.f;
        glm::vec2 size = end-start;
        
        View::Instance()->GetPrimitiveRenderer()->AttachBox(layer, Vec2(position.x, position.y), Vec2(size.x, size.y), Vec3(0,0,0), Vec4(0,0,0.1,0.1));
        View::Instance()->GetPrimitiveRenderer()->AttachBox(layer, Vec2(position.x, position.y), Vec2(size.x, size.y), Vec3(0,0,0), Vec4(0,0,0.5,0.5), false);
    }
}

bool SelectManipulator::OnMouseDown(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position)
{
    if (button == pb::kMouseButtonLeft)
    {
        _Active = true;
        _SelectStart = _SelectEnd = position;
        
        return true;
    }

    return false;
}

bool SelectManipulator::OnMouseUp(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position)
{
    if (_Active && button == pb::kMouseButtonLeft)
    {
        _Active = false;
        _SelectEnd = position;
        glm::vec2 start = View::Instance()->GetLevelCamera()->ConvertScreenToWorld(_SelectStart);
        glm::vec2 end = View::Instance()->GetLevelCamera()->ConvertScreenToWorld(_SelectEnd);
        glm::vec2 min(glm::min(start.x, end.x), glm::min(start.y, end.y));
        glm::vec2 max(glm::max(start.x, end.x), glm::max(start.y, end.y));
        
        Level* level = View::Instance()->GetLevel();
        Level::EntityMap entities = level->GetEntities();
        
        std::string entityIds;
        
        pb::BoundingBox selectionBounds(glm::vec3(min, 0), glm::vec3(max, 0));

        bool singleSelection = min == max;
        
        for (Level::EntityMap::iterator it = entities.begin(); it != entities.end(); ++it)
        {
            if (it->second->GetBoundingBox().Intersects(selectionBounds))
            {
                char id[32];
                sprintf(id, "%d", it->first);
                
                entityIds += std::string((entityIds.length() == 0 ? "" : ",")) + id;
                
                if (singleSelection)
                {
                    break;
                }
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
    
    return false;
}

bool SelectManipulator::OnMouseMove(glm::vec2 position)
{
    _SelectEnd = position;
    return false;
}

bool SelectManipulator::OnKeyDown(pb::KeyboardKey key, char character)
{
    return false;
}

bool SelectManipulator::OnKeyUp(pb::KeyboardKey key, char character)
{
    return false;
}

void SelectManipulator::OnSetActive()
{
    
}

void SelectManipulator::OnSetInactive()
{
    
}
