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

class Vec2;

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
        bool OnExport(pixelboost::HttpConnection& connection);
        bool OnCreateRecord(pixelboost::HttpConnection& connection, const std::string& name, const std::string& type);
        bool OnCreateEntity(pixelboost::HttpConnection& connection, Uid recordId, const std::string& type);
        bool OnSetTransform(pixelboost::HttpConnection& connection, Uid recordId, Uid entityId, const Vec2& position, float rotation, const Vec2& scale);
        
        bool OnGetRecords(pixelboost::HttpConnection& connection);
        bool OnGetRecord(pixelboost::HttpConnection& connection, Uid record);
        bool OnGetEntity(pixelboost::HttpConnection& connection, Uid record, Uid entity);

        bool OnGetSchema(pixelboost::HttpConnection& connection);
        
        bool OnGetImage(pixelboost::HttpConnection& connection, const std::string& image);
        
        void InsertSchemaItem(json::Array& array, SchemaStruct* schemaItem);
        void ExportProperty(json::Object& property, const std::string& name, const SchemaProperty* schemaProp);
        
        friend class DebugVariable;
    };    
}
