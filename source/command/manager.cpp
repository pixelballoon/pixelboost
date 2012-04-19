#include <iostream>

#include "command/base.h"
#include "command/manager.h"

using namespace pixeleditor;

CommandManager::CommandManager()
{
    
}

CommandManager::~CommandManager()
{
    ClearUndoStack();
    ClearRedoStack();
}

void CommandManager::RegisterCommand(const std::string& command, CreateCommand createCommand)
{
    _Commands[command] = createCommand;
}

void CommandManager::Exec(const std::string& command, const std::string& arguments)
{
    std::cout << "cmd." << command << "(\"" << arguments << "\");" << std::endl;       
    
    ClearRedoStack();
    
    if (_Commands.find(command) == _Commands.end())
    {
        std::cout << "// Command doesn't exist" << std::endl;
        return;
    }
    
    Command* cmd = _Commands[command]();
    
    cmd->SetArguments(arguments);
    
    if (cmd->Do())
    {
        std::cout << "// Succeeded" << std::endl;
        
        if (cmd->CanUndo())
            _UndoStack.push_back(cmd);
    }
    else
        std::cout << "// Failed" << std::endl;
}

bool CommandManager::Undo()
{
    if (_UndoStack.size() == 0)
    {
        std::cout << "// Undo stack empty, cannot undo" << std::endl;
        return false;
    }
    
    Command* cmd = _UndoStack.back();
    
    std::cout << "// Undoing " << cmd->GetName() << std::endl;
    
    bool result = cmd->Undo();
    
    if (result)
    {
        _UndoStack.pop_back();
        _RedoStack.push_back(cmd);
    } else {
        std::cout << "// Undo failed" << std::endl;
    }
    
    return result;
}

bool CommandManager::Redo()
{
    if (_RedoStack.size() == 0)
    {
        std::cout << "// Redo stack empty, cannot redo" << std::endl;
        return false;
    }
    
    Command* cmd = _RedoStack.back();
    
    std::cout << "// Redoing " << cmd->GetName() << std::endl;
    
    bool result = cmd->Do();
    
    if (result)
    {
        _RedoStack.pop_back();
        _UndoStack.push_back(cmd);
    }    
    
    return result;
}

void CommandManager::ClearUndoStack()
{
    for (CommandList::iterator it = _UndoStack.begin(); it != _UndoStack.end(); ++it)
    {
        delete *it;
    }
    _UndoStack.clear();
}

void CommandManager::ClearRedoStack()
{
    for (CommandList::iterator it = _RedoStack.begin(); it != _RedoStack.end(); ++it)
    {
        delete *it;
    }
    _RedoStack.clear();    
}
