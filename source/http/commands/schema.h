#pragma once

#include <string>

namespace json
{
    class Array;
    class Object;
}

namespace pb
{
    class HttpConnection;
}

namespace pixeleditor
{
    class SchemaItem;
    class SchemaStruct;
    class SchemaProperty;
    
    class SchemaCommands
    {
    public:
        SchemaCommands();
        
    private:
        bool OnGetSchema(pb::HttpConnection& connection, const std::vector<std::string>& urlArguments, const std::map<std::string, std::string>& queryArguments);
        void InsertSchemaItem(json::Array& array, SchemaStruct* schemaItem);
        void ExportAttributes(json::Object& parent, const SchemaItem* schemaItem);
        void ExportProperty(json::Object& property, const std::string& name, const SchemaProperty* schemaProp);
    };
}
