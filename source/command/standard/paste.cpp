#include <sstream>
#include <string>

#include "command/standard/paste.h"
#include "core/uidHelpers.h"
#include "project/entity.h"
#include "project/project.h"
#include "project/record.h"
#include "project/schema.h"
#include "core.h"

using namespace pixeleditor;

Command* PasteCommand::Create()
{
    return new PasteCommand();
}

std::string PasteCommand::GetStaticName()
{
    return "paste";
}

PasteCommand::PasteCommand()
{
    
}

PasteCommand::~PasteCommand()
{
}

std::string PasteCommand::GetName()
{
    return PasteCommand::GetStaticName();
}

bool PasteCommand::CanUndo()
{
    return true;
}

bool PasteCommand::Do(std::string& returnString)
{
    Core* core = Core::Instance();
    
    Uid recordId = atoi(GetArgument("r").c_str());
    
    ProjectRecord* record = core->GetProject()->GetRecord(recordId);
    
    if (!record)
        return false;
    
    json::Object* clipboard = core->GetClipboard();
    
    if (!clipboard)
        return false;
    
    std::stringstream test;
    json::Writer::Write(clipboard, test);
    printf("%s\n", test.str().c_str());
    
    _Selection = Core::Instance()->GetSelection();
    
    Selection newSelection;
    
    if (static_cast<json::String>((*clipboard)["type"]).Value() == "entities")
    {
        json::Array entities = (*clipboard)["entities"];
        
        for (json::Array::iterator it = entities.Begin(); it != entities.End(); ++it)
        {   
            ProjectEntity* entity = new ProjectEntity(record);
            entity->Open(*it, true);
            record->AddEntity(entity);
            
            newSelection.AddEntity(GenerateSelectionUid(entity->GetUid()));
        }
    }
    
    Core::Instance()->SetSelection(newSelection);
    
    return true;
}

bool PasteCommand::Undo()
{
    Core::Instance()->SetSelection(_Selection);
    
    return false;
}
