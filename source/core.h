#pragma once

#include <string>

#include "sigslot/signal.h"

#include "core/selection.h"

namespace pb
{
    class NetworkServer;
}

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
    
    void Update();
    
    CommandManager* GetCommandManager();
    HttpInterface* GetHttpInterface();
    pb::NetworkServer* GetNetworkManager();

    Project* GetProject();
    
    Selection& GetSelection();
    void SetSelection(const Selection& selection);
    
private:
    void RegisterCoreCommands();
    
private:
    static Core* _Instance;
    
    HttpInterface* _HttpInterface;
    
    CommandManager* _CommandManager;
    pb::NetworkServer* _NetworkManager;
    
    Project* _Project;
    
    Selection _Selection;
};
    
}
