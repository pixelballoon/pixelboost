#pragma once

#include <string>

#include "sigslot/signal.h"

#include "core/selection.h"

namespace pixeleditor
{

class CommandManager;
class HttpInterface;
class Project;

class Core
{
public:
    Core();
    ~Core();
    
public:
    static Core* Instance();
    
    CommandManager* GetCommandManager();
    HttpInterface* GetHttpInterface();

    Project* GetProject();
    
    Selection& GetSelection();
    void SetSelection(const Selection& selection);
    
private:
    void RegisterCoreCommands();
    
private:
    static Core* _Instance;
    
    HttpInterface* _HttpInterface;
    
    CommandManager* _CommandManager;
    
    Project* _Project;
    
    Selection _Selection;
};
    
}
