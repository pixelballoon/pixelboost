#include <string>

#include "command/standard/selectCommand.h"
#include "core/selection.h"
#include "core.h"

using namespace pixeleditor;

Command* SelectCommand::Create()
{
    return new SelectCommand();
}

std::string SelectCommand::GetStaticName()
{
    return "select";
}

std::string SelectCommand::GetName()
{
    return SelectCommand::GetStaticName();
}

SelectCommand::SelectCommand()
{
    
}

SelectCommand::~SelectCommand()
{
    
}

bool SelectCommand::CanUndo()
{
    return true;
}

bool SelectCommand::Do()
{
    _Selection = Core::Instance()->GetSelection();
    
    Selection newSelection;
    
    if (IsArgumentSet("c"))
    {
        Core::Instance()->SetSelection(newSelection);
        return true;
    }
    
    if (IsArgumentSet("a"))
        newSelection = _Selection;
    
    if (IsArgumentSet("i"))
    {
        unsigned long count = GetArgumentCount("i");
        
        for (int i=0; i<count; i++)
        {
            std::string id = GetArgument("i", i);
            
            newSelection.AddEntity(atoi(id.c_str()));
        }
        
        Core::Instance()->SetSelection(newSelection);
        
        return true;
    }
    
    return false;
}

bool SelectCommand::Undo()
{
    Core::Instance()->SetSelection(_Selection);
    
    return true;
}
