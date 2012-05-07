#pragma once

#include <string>

namespace json
{
    class Array;
    class Object;
}

namespace pixelboost
{
    class HttpConnection;
}

namespace pixeleditor
{
    class SchemaStruct;
    class SchemaProperty;
    
    class SchemaCommands
    {
    public:
        SchemaCommands();
        
    private:
        bool OnGetSchema(pixelboost::HttpConnection& connection, const std::vector<std::string>& urlArguments, const std::map<std::string, std::string>& queryArguments);
        void InsertSchemaItem(json::Array& array, SchemaStruct* schemaItem);
        void ExportProperty(json::Object& property, const std::string& name, const SchemaProperty* schemaProp);
    };
}
