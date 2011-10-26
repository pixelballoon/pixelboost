#ifndef LIBPIXEL__DEBUG__DEBUGVARIABLEMANAGER__H
#define LIBPIXEL__DEBUG__DEBUGVARIABLEMANAGER__H

#include <map>
#include <string>

#include "libpixel/network/http/httpServer.h"

namespace json
{
    class Object;
}

namespace libpixel
{
    
class DebugVariable;

class DebugVariableManager : public HttpServer
{
public:
    DebugVariableManager(const std::string& htmlLocation = "libpixel/debug/html");
    ~DebugVariableManager();
    
    static DebugVariableManager* Instance();
    
private:
#ifndef PIXELBALLOON_BUILD_RELEASE    
    typedef std::map<int, DebugVariable*> VariableMap;
    
    void AddVariable(DebugVariable* variable);
    void SendValue(DebugVariable* variable);
    
    virtual bool OnHttpRequest(RequestType type, const std::string& uri, const std::string& query, const std::string& data, HttpConnection& connection);
    
    void OnGetVariables(HttpConnection& connection);
    void OnGetVariable(HttpConnection& connection, DebugVariable* variable);
    void OnSetVariable(HttpConnection& connection, DebugVariable* variable, json::Object& params);
    
    void PopulateVariable(DebugVariable* variable, json::Object& o);
    
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