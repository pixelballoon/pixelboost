#include "core.h"
#include "command/manager.h"
#include "http/httpInterface.h"
#include "project/project.h"

using namespace pixeleditor;

Core* Core::_Instance = 0;

Core::Core()
{
    _Instance = this;
    
    _HttpInterface = new HttpInterface();
    
    _CommandManager = new CommandManager();
    _Project = new Project();
    
    RegisterCoreCommands();
}

Core::~Core()
{
    _Instance = 0;
    
    delete _CommandManager;
    delete _Project;
}

Core* Core::Instance()
{
    return _Instance;
}

CommandManager* Core::GetCommandManager()
{
    return _CommandManager;
}

Project* Core::GetProject()
{
    return _Project;
}
    
const Selection& Core::GetSelection()
{
    return _Selection;
}

void Core::SetSelection(const Selection& selection)
{
    _Selection = selection;
    
    selectionChanged(&_Selection);
}
