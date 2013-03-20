#pragma once

#include <map>
#include <string>
#include <vector>

class Command;

#define REGISTER_COMMAND(a) Core::Instance()->GetCommandManager()->RegisterCommand(a::GetStaticName(), &a::Create);

class CommandManager
{
public:
    CommandManager();
    ~CommandManager();
    
    typedef Command*(*CreateCommand)();
    
    void RegisterCommand(const std::string& command, CreateCommand createCommand);
    std::string Exec(const std::string& command, const std::string& arguments = "");
    
    bool Undo();
    bool Redo();
    
private:
    void ClearUndoStack();
    void ClearRedoStack();
    
    typedef std::vector<Command*> CommandList;
    
    std::map<std::string, CreateCommand> _Commands;
    
    CommandList _UndoStack;
    CommandList _RedoStack;
};
