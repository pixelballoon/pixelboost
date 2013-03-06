#include "pixelboost/graphics/camera/camera.h"

#include "command/manager.h"
#include "project/entity.h"
#include "project/record.h"
#include "project/project.h"
#include "project/property.h"
#include "view/entity/entity.h"
#include "view/manipulator/create.h"
#include "view/manipulator/move.h"
#include "view/level.h"
#include "core.h"
#include "view.h"

PB_DEFINE_ENTITY(CreateManipulator)

CreateManipulator::CreateManipulator(pb::Scene* scene, pb::Entity* parent, pb::DbEntity* creationEntity)
    : Manipulator(scene, parent, creationEntity)
    , _CreateMode(false)
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
        
        glm::vec3 position = GetEntityPosition(start);
        
        if (View::Instance()->GetRecord())
        {
            _CreateMode = true;
            _LastCreate = position;
            
            CreateEntity(position);
            
            return true;
        }
    } else if (button == pb::kMouseButtonRight)
    {
        View::Instance()->GetManipulatorManager()->SetActiveManipulator("select");
        return true;
    }
    
    return false;
}

bool CreateManipulator::OnMouseUp(pb::MouseButton button, pb::ModifierKeys modifierKeys, glm::vec2 position)
{
    if (button == pb::kMouseButtonLeft)
    {
        _CreateMode = false;
    }
    
    return false;    
}

bool CreateManipulator::OnMouseMove(glm::vec2 position)
{
    if (_CreateMode)
    {
        glm::vec2 start = View::Instance()->GetLevelCamera()->ConvertScreenToWorld(position);
        
        glm::vec3 position = GetEntityPosition(start);
        
        if (glm::distance(position, _LastCreate) >= 1.f)
        {
            _LastCreate = position;
            CreateEntity(position);
        }
    }
    return false;
}

bool CreateManipulator::OnKeyDown(pb::KeyboardKey key, pb::ModifierKeys modifier, char character)
{
    return false;
}

bool CreateManipulator::OnKeyUp(pb::KeyboardKey key, pb::ModifierKeys modifier, char character)
{
    return false;
}

glm::vec3 CreateManipulator::GetEntityPosition(glm::vec2 start)
{
    glm::vec3 positionSnap = static_cast<MoveManipulator*>(View::Instance()->GetManipulatorManager()->GetManipulator("move"))->GetSnap();
    
    glm::vec3 position = glm::vec3(start, 0.f);
    if (positionSnap.x != 0)
        position.x = position.x - glm::mod(position.x, positionSnap.x);
    if (positionSnap.y != 0)
        position.y = position.y - glm::mod(position.y, positionSnap.y);
    if (positionSnap.z != 0)
        position.z = position.z - glm::mod(position.z, positionSnap.z);
    
    return position;
}

void CreateManipulator::CreateEntity(glm::vec3 position)
{
    char commandArgs[1024];
    sprintf(commandArgs, "-r %d -t %s -p %f,%f,%f", View::Instance()->GetRecord()->GetUid(), _EntityType.c_str(), position.x, position.y, position.z);
    std::string entityIdString = Core::Instance()->GetCommandManager()->Exec("createEntity", commandArgs);
    Uid entityId = atoi(entityIdString.c_str());
    ProjectEntity* entity = Core::Instance()->GetProject()->GetEntity(entityId);
    
    if (entity)
    {
        for (int i=0; i<_Fields.size(); i++)
        {
            entity->AcquireAtom(_Fields[i])->SetStringValue(_Values[i]);
        }
        
        Core::Instance()->GetCommandManager()->Exec("select", "-u " + entityIdString);
    }
    
    ViewEntity* viewEntity = View::Instance()->GetLevel()->GetEntityById(entityId);
    
    if (viewEntity)
    {
        Level::EntityList otherEntities = View::Instance()->GetLevel()->GetEntitiesInBounds(viewEntity->GetBoundingBox());
        
        if (otherEntities.size())
        {
            float maxDepth = -INFINITY;
            for (Level::EntityList::iterator it = otherEntities.begin(); it != otherEntities.end(); ++it)
            {
                maxDepth = glm::max(maxDepth, (*it)->GetPosition().z);
            }
            
            viewEntity->Transform(glm::vec3(0,0, (maxDepth + 1)));
            viewEntity->CommitTransform();
        }
    }
}