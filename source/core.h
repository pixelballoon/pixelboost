#pragma once

#include <string>

#include "pixelboost/misc/delegate/signal.h"

#include "manipulator/selection.h"

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
    
    const Selection& GetSelection();
    void SetSelection(const Selection& selection);
    
    sigslot::Signal1<const Selection*> selectionChanged;
    
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
