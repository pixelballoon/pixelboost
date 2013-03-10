#pragma once

#include <map>
#include <string>

#include "pixelboost/network/networkServer.h"

namespace json
{
    class Object;
}

namespace pb
{
    
class DebugVariable;

class DebugVariableManager : public NetworkHandler
{
public:
    DebugVariableManager(const std::string& htmlLocation = "data/pixelboost/debug/html");
    virtual ~DebugVariableManager();
    
    static DebugVariableManager* Instance();
    
private:
#ifndef PIXELBOOST_BUILD_RELEASE    
    typedef std::map<std::string, DebugVariable*> VariableMap;
    
    void AddVariable(DebugVariable* variable);
    const VariableMap& GetVariables() const;
    
    virtual void OnReceive(NetworkConnection& connection, NetworkMessage& message);
    
    void SendValue(DebugVariable* variable);
#endif
    
private:
    static DebugVariableManager* _Instance;
    
#ifndef PIXELBOOST_BUILD_RELEASE
    VariableMap _Variables;
#endif
    
    friend class DebugVariable;
};
    
}
