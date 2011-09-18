#ifndef LIBPIXEL__DEBUG__DEBUGVARIABLEMANAGER__H
#define LIBPIXEL__DEBUG__DEBUGVARIABLEMANAGER__H

#include <map>
#include <string>

#include "libpixel/debug/debugNetworkManager.h"

namespace libpixel
{
    
class DebugVariable;

class DebugVariableManager : public DebugNetworkHandler
{
public:
    DebugVariableManager();
    ~DebugVariableManager();
    
    static DebugVariableManager* Instance();
    
private:
#ifndef PIXELBALLOON_BUILD_RELEASE    
    typedef std::map<std::string, DebugVariable*> VariableMap;
    
    void AddVariable(DebugVariable* variable);
    void VariableChanged(DebugVariable* variable);
    void SendValue(DebugVariable* variable);
    
    virtual void OnReceive(DebugConnection& connection, NetworkMessage& message);
    
    const VariableMap& GetVariables() const;
#endif
    
private:
    static DebugVariableManager* _Instance;
    
#ifndef PIXELBALLOON_BUILD_RELEASE
    VariableMap _Variables;
#endif
    
    friend class DebugVariable;
};
    
}

#endif