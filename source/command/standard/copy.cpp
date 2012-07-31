#include <string>

#include "command/standard/copy.h"
#include "project/entity.h"
#include "project/project.h"
#include "project/record.h"
#include "project/schema.h"
#include "view/entity/entity.h"
#include "view/level.h"
#include "core.h"
#include "view.h"

using namespace pixeleditor;

Command* CopyCommand::Create()
{
    return new CopyCommand();
}

std::string CopyCommand::GetStaticName()
{
    return "copy";
}

CopyCommand::CopyCommand()
{
    
}

CopyCommand::~CopyCommand()
{
}

std::string CopyCommand::GetName()
{
    return CopyCommand::GetStaticName();
}

bool CopyCommand::CanUndo()
{
    return false;
}

bool CopyCommand::Do(std::string& returnString)
{
    json::Object clipboard;
    
    clipboard["type"] = json::String("entities");
    
    json::Array serialisedEntities;
    
    Core* core = Core::Instance();
    View* view = View::Instance();
    
    Selection& selection = core->GetSelection();
    
    const Selection::Entities& entities = selection.GetSelection();
    
    for (Selection::Entities::const_iterator it = entities.begin(); it != entities.end(); ++it)
    {
        json::Object serialisedEntity;
        
        ViewEntity* viewEntity = view->GetLevel()->GetEntityById(it->first);
        
        if (!viewEntity)
            continue;
        
        Entity* entity = viewEntity->GetEntity();
        
        entity->Save(serialisedEntity);
        
        serialisedEntities.Insert(serialisedEntity);
    }
    
    clipboard["entities"] = serialisedEntities;
    
    core->SetClipboard(clipboard);
    
    return true;
}
