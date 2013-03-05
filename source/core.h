#pragma once

#include <string>

#include "sigslot/signal.h"

#include "core/selection.h"

namespace json
{
    class Object;
}

namespace pb
{
    class NetworkServer;
}

class CommandManager;
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
    pb::NetworkServer* GetNetworkManager();

    Project* GetProject();
    
    Selection& GetSelection();
    void SetSelection(const Selection& selection);
    
    json::Object* GetClipboard();
    void SetClipboard(const json::Object& clipboard);
    
private:
    void RegisterCoreCommands();
    
private:
    static Core* _Instance;
    
    CommandManager* _CommandManager;
    pb::NetworkServer* _NetworkManager;
    
    Project* _Project;
    
    Selection _Selection;
    
    json::Object* _Clipboard;
};
