#include <string>

#include "command/standard/deleteCommand.h"
#include "core/selection.h"
#include "project/entity.h"
#include "project/project.h"
#include "project/record.h"
#include "project/schema.h"
#include "core.h"

using namespace pixeleditor;

Command* DeleteCommand::Create()
{
    return new DeleteCommand();
}

std::string DeleteCommand::GetStaticName()
{
    return "delete";
}
    
DeleteCommand::DeleteCommand()
{
    
}
    
DeleteCommand::~DeleteCommand()
{
    for (std::vector<std::pair<Uid, Entity*> >::iterator it = _Entities.begin(); it != _Entities.end(); ++it)
    {
        delete it->second;
    }
    _Entities.clear();
}

std::string DeleteCommand::GetName()
{
    return DeleteCommand::GetStaticName();
}

bool DeleteCommand::CanUndo()
{
    return true;
}

bool DeleteCommand::Do(std::string& returnString)
{
    const Selection& selection = Core::Instance()->GetSelection();
    
    Project* project = Core::Instance()->GetProject();
    
    for (Selection::Entities::const_iterator it = selection.GetSelection().begin(); it != selection.GetSelection().end(); ++it)
    {
        Entity* entity = project->GetEntity(it->first);
        if (entity)
        {
            Record* record = entity->GetRecord();
            record->RemoveEntity(entity, false);
            _Entities.push_back(std::pair<Uid, Entity*>(record->GetUid(), entity));
        }
    }
    
    return true;
}

bool DeleteCommand::Undo()
{
    Project* project = Core::Instance()->GetProject();

    for (std::vector<std::pair<Uid, Entity*> >::iterator it = _Entities.begin(); it != _Entities.end(); ++it)
    {
        Record* record = project->GetRecord(it->first);
        // TODO: Assert record is valid
        record->AddEntity(it->second);
    }
    _Entities.clear();
    
    return true;
}
