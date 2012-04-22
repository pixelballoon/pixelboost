#pragma once

#include <map>
#include <string>

#include "pixelboost/network/http/httpServer.h"

#include "project/definitions.h"

namespace json
{
    class Array;
    class Object;
}

namespace pixelboost
{
    class DebugVariable;
}

namespace pixeleditor
{
    class SchemaProperty;
    class SchemaStruct;
    
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
        
        bool OnSave(pixelboost::HttpConnection& connection);
        bool OnCreateRecord(pixelboost::HttpConnection& connection, const std::string& name, const std::string& type);
        bool OnCreateEntity(pixelboost::HttpConnection& connection, Uid recordId, const std::string& type);
        
        bool OnGetRecords(pixelboost::HttpConnection& connection);
        bool OnGetEntities(pixelboost::HttpConnection& connection, Uid record);

        bool OnGetSchema(pixelboost::HttpConnection& connection);
        bool OnGetSchema(pixelboost::HttpConnection& connection, const std::string& item);
        
        void InsertSchemaItem(json::Array& array, SchemaStruct* schemaItem);
        void ExportProperty(json::Object& property, const std::string& name, const SchemaProperty* schemaProp);
        
        friend class DebugVariable;
    };    
}
