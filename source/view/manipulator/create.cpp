#include "pixelboost/graphics/camera/camera.h"

#include "command/manager.h"
#include "project/record.h"
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

void CreateManipulator::SetActorType(const std::string& actor)
{
    _ActorType = actor;
}

bool CreateManipulator::OnMouseDown(pb::MouseButton button, glm::vec2 position)
{
    if (button == pb::kMouseButtonLeft)
    {
        glm::vec2 start = View::Instance()->GetLevelCamera()->ConvertScreenToWorld(position);
        
        char commandArgs[1024];
        sprintf(commandArgs, "-r %d -t %s -p %f,%f,%f", View::Instance()->GetRecord()->GetUid(), _ActorType.c_str(), start.x, start.y, 0.f);
        Core::Instance()->GetCommandManager()->Exec("createEntity", commandArgs);
        
        return true;
    } else if (button == pb::kMouseButtonRight)
    {
        View::Instance()->GetManipulatorManager()->SetActiveManipulator("select");
        return true;
    }
    
    return false;
}

bool CreateManipulator::OnMouseUp(pb::MouseButton button, glm::vec2 position)
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
