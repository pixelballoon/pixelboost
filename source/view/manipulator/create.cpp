#include "pixelboost/graphics/camera/camera.h"

#include "command/manager.h"
#include "project/entity.h"
#include "project/record.h"
#include "project/project.h"
#include "project/property.h"
#include "view/manipulator/create.h"
#include "core.h"
#include "view.h"

using namespace pixeleditor;

CreateManipulator::CreateManipulator()
{
    
}

CreateManipulator::~CreateManipulator()
{
    
}

std::string CreateManipulator::GetName()
{
    return "create";
}

void CreateManipulator::SetEntityType(const std::string& entity)
{
    _EntityType = entity;
    _Fields.clear();
    _Values.clear();
}

void CreateManipulator::SetCreationData(const std::vector<std::string>& fields, const std::vector<std::string>& values)
{
    _Fields = fields;
    _Values = values;
}

bool CreateManipulator::OnMouseDown(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position)
{
    if (button == pb::kMouseButtonLeft)
    {
        glm::vec2 start = View::Instance()->GetLevelCamera()->ConvertScreenToWorld(position);
        
        char commandArgs[1024];
        sprintf(commandArgs, "-r %d -t %s -p %f,%f,%f", View::Instance()->GetRecord()->GetUid(), _EntityType.c_str(), start.x, start.y, 0.f);
        std::string entityIdString = Core::Instance()->GetCommandManager()->Exec("createEntity", commandArgs);
        Uid entityId = atoi(entityIdString.c_str());
        Entity* entity = Core::Instance()->GetProject()->GetEntity(entityId);
        
        for (int i=0; i<_Fields.size(); i++)
        {
            entity->AcquireAtom(_Fields[i])->SetStringValue(_Values[i]);
        }
        
        Core::Instance()->GetCommandManager()->Exec("select", "-u " + entityIdString);
        
        return true;
    } else if (button == pb::kMouseButtonRight)
    {
        View::Instance()->GetManipulatorManager()->SetActiveManipulator("select");
        return true;
    }
    
    return false;
}

bool CreateManipulator::OnMouseUp(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position)
{
    return false;    
}

bool CreateManipulator::OnMouseMove(glm::vec2 position)
{
    return false;
}

bool CreateManipulator::OnKeyDown(pb::KeyboardKey key, char character)
{
    return false;
}

bool CreateManipulator::OnKeyUp(pb::KeyboardKey key, char character)
{
    return false;
}
