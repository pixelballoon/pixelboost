#pragma once

#include <map>
#include <string>

#include "pixelboost/network/http/httpServer.h"

#include "project/definitions.h"

namespace json
{
    class Object;
}

namespace pixelboost
{
    class DebugVariable;
}

namespace pixeleditor
{
    class HttpInterface : public pixelboost::HttpServer
    {
    public:
        HttpInterface(const std::string& htmlLocation = "data/html");
        ~HttpInterface();
        
    private: 
        typedef std::map<int, pixelboost::DebugVariable*> VariableMap;
        
        void AddVariable(pixelboost::DebugVariable* variable);
        void SendValue(pixelboost::DebugVariable* variable);
        
        virtual bool OnHttpRequest(RequestType type, const std::string& uri, const std::string& query, const std::string& data, pixelboost::HttpConnection& connection);
        
        bool OnGetRecords(pixelboost::HttpConnection& connection);
        bool OnGetEntities(pixelboost::HttpConnection& connection, Uid record);
        
        friend class DebugVariable;
    };    
}
