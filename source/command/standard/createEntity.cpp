#include <string>

#include "command/standard/createEntity.h"
#include "project/entity.h"
#include "project/project.h"
#include "project/record.h"
#include "project/schema.h"
#include "core.h"

using namespace pixeleditor;

Command* CreateEntityCommand::Create()
{
    return new CreateEntityCommand();
}

std::string CreateEntityCommand::GetStaticName()
{
    return "createEntity";
}
    
CreateEntityCommand::CreateEntityCommand()
    : _EntityUid(0)
{
    
}
    
CreateEntityCommand::~CreateEntityCommand()
{
}

std::string CreateEntityCommand::GetName()
{
    return CreateEntityCommand::GetStaticName();
}

bool CreateEntityCommand::CanUndo()
{
    return true;
}

bool CreateEntityCommand::Do(std::string& returnString)
{
    Core* core = Core::Instance();
    
    _RecordId = atoi(GetArgument("r").c_str());
    
    ProjectRecord* record = core->GetProject()->GetRecord(_RecordId);
    
    if (!record)
        return false;
    
    std::string entityType = GetArgument("t");
    
    const SchemaEntity* type = core->GetProject()->GetSchema()->GetEntityByName(entityType);
    
    if (!type)
        return false;
    
    float x = (float)atof(GetArgument("p", 0).c_str());
    float y = (float)atof(GetArgument("p", 1).c_str());
    float z = (float)atof(GetArgument("p", 2).c_str());
    
    ProjectEntity* entity = new ProjectEntity(record, type);

    if (_EntityUid)
        entity->SetUid(_EntityUid);
    else
        _EntityUid = entity->GetUid();
    
    entity->SetName(type->GetName());
    entity->SetPosition(glm::vec3(x, y, z));
    
    record->AddEntity(entity);
    
    char entityId[32];
    sprintf(entityId, "%d", _EntityUid);
    returnString = entityId;
    
    return true;
}

bool CreateEntityCommand::Undo()
{
    Core* core = Core::Instance();
    
    ProjectRecord* record = core->GetProject()->GetRecord(_RecordId);

    ProjectEntity* entity = record->GetEntity(_EntityUid);
    
    record->RemoveEntity(entity);
    
    return true;
}
