#pragma once

#include <map>
#include <string>

#include "glm/glm.hpp"
#include "srutil/delegate/delegate.hpp"

#include "pixelboost/network/http/httpServer.h"

#include "project/definitions.h"

namespace json
{
    class Array;
    class Object;
}

namespace pb
{
    class DebugVariable;
}

namespace pixeleditor
{
    class FileCommands;
    class Property;
    class RecordCommands;
    class SchemaCommands;
    class SchemaProperty;
    class SchemaStruct;
    
    class HttpInterface : public pb::HttpServer
    {
    public:
        HttpInterface();
        ~HttpInterface();
        
        typedef srutil::delegate3<bool, pb::HttpConnection&, const std::vector<std::string>&, const std::map<std::string, std::string>&> CommandDelegate;
        
        void Initialise();
        void RegisterCommand(const std::string& command, HttpServer::RequestType requestType, CommandDelegate delegate);
        
    private: 
        typedef std::map<int, pb::DebugVariable*> VariableMap;
        
        void AddVariable(pb::DebugVariable* variable);
        void SendValue(pb::DebugVariable* variable);
        
        virtual bool OnHttpRequest(RequestType type, const std::string& uri, const std::string& query, const std::string& data, pb::HttpConnection& connection);
        
        bool OnSave(pb::HttpConnection& connection);
        bool OnExport(pb::HttpConnection& connection);
        bool OnCreateRecord(pb::HttpConnection& connection, const std::string& name, const std::string& type);
        bool OnCreateEntity(pb::HttpConnection& connection, Uid recordId, const std::string& type);
        bool OnSetTransform(pb::HttpConnection& connection, Uid recordId, Uid entityId, const glm::vec3& position, float rotation, const glm::vec3& scale);
        
        bool OnGetRecords(pb::HttpConnection& connection);
        bool OnGetRecord(pb::HttpConnection& connection, Uid recordId);
        bool OnGetEntity(pb::HttpConnection& connection, Uid recordId, Uid entityId);

        bool OnGetRecordProperty(pb::HttpConnection& connection, Uid recordId, const std::string& path);
        bool OnGetEntityProperty(pb::HttpConnection& connection, Uid recordId, Uid entityId, const std::string& path);
        bool OnGetStructProperty(pb::HttpConnection& connection, const Property* property);
        
        bool OnSetRecordProperty(pb::HttpConnection& connection, Uid recordId, const std::string& path, const std::string& type, const std::string& value);
        bool OnSetEntityProperty(pb::HttpConnection& connection, Uid recordId, Uid entityId, const std::string& path, const std::string& type, const std::string& value);

        bool OnGetSchema(pb::HttpConnection& connection);
        
        bool OnGetImage(pb::HttpConnection& connection, const std::string& image);
        
        void InsertSchemaItem(json::Array& array, SchemaStruct* schemaItem);
        void ExportProperty(json::Object& property, const std::string& name, const SchemaProperty* schemaProp);
        
        typedef std::pair<std::string, HttpServer::RequestType> CommandRequest;
        typedef std::map<CommandRequest, CommandDelegate> CommandMap;
        
        CommandMap _Commands;
        
    private:
        FileCommands* _FileCommands;
        SchemaCommands* _SchemaCommands;
        RecordCommands* _RecordCommands;
    };    
}
