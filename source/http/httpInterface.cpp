#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "pixelboost/data/json/writer.h"
#include "pixelboost/debug/debugVariable.h"
#include "pixelboost/debug/debugVariableManager.h"
#include "pixelboost/file/fileHelpers.h"
#include "pixelboost/network/networkMessage.h"

#include "command/manager.h"
#include "http/httpInterface.h"
#include "project/entity.h"
#include "project/project.h"
#include "project/record.h"
#include "project/schema.h"
#include "core.h"

using namespace pixelboost;
using namespace pixeleditor;

HttpInterface::HttpInterface(const std::string& htmlLocation)
    : pixelboost::HttpServer()
{
    Start(9090, pixelboost::FileHelpers::GetRootPath()+"/"+htmlLocation);
}

HttpInterface::~HttpInterface()
{
    
}

bool HttpInterface::OnHttpRequest(HttpServer::RequestType type, const std::string& uri, const std::string& query, const std::string& data, pixelboost::HttpConnection& connection)
{
    ssize_t split = uri.find('/', 1);
    
    std::string command;
    std::string argumentString;
    std::string queryString = query;
    std::vector<std::string> urlArguments;
    std::map<std::string, std::string> queryArguments;
    
    if (split > 0)
    {
        command = uri.substr(1, split-1);
        argumentString = uri.substr(split);
    } else {
        command = uri.substr(1);
    }
        
    while (argumentString.length() > 1)
    {
        ssize_t split = argumentString.find('/', 1);
        
        std::string argument;
        
        if (split > 0)
        {
            argument = argumentString.substr(1, split-1);
            argumentString = argumentString.substr(split);
        } else {
            argument = argumentString.substr(1);
            argumentString = "";
        }
        
        if (argument.length() > 0)
            urlArguments.push_back(argument);
        else
            break;
    }
    
    while (queryString.length() > 1)
    {
        ssize_t split = queryString.find('&', 1);
        
        std::string argument;
        
        if (split > 0)
        {
            argument = queryString.substr(0, split);
            queryString = queryString.substr(split+1);
        } else {
            argument = queryString.substr(0);
            queryString = "";
        }
        
        if (argument.length() > 0)
        {
            std::string field;
            std::string value;
            
            int op = argument.find('=');
            if (op > 0)
            {
                field = argument.substr(0, op);
                value = argument.substr(op+1);
            }
            queryArguments[field] = value;
        }
        else
            break;
    }
    
    
    
    bool replied = false;
    
    if (command == "images" && type == kRequestTypeGet)
    {
        connection.AddHeader("Content-Type", "image/png");
        
        if (urlArguments.size() == 1)
        {
            replied = OnGetImage(connection, urlArguments[0]);
        }
    } else {
        connection.AddHeader("Content-Type", "application/json;charset=utf-8");
    }
    
    if (command == "records" && type == kRequestTypeGet)
    {
        replied = OnGetRecords(connection);
    } else if (command == "record" && type == kRequestTypeGet)
    {
        if (urlArguments.size() == 1)
        {
            std::string record = urlArguments[0];
            replied = OnGetRecord(connection, atoi(record.c_str()));
        }
    } else if (command == "schema" && type == kRequestTypeGet)
    {
        replied = OnGetSchema(connection);
    } else if (command == "record" && (type == kRequestTypePost || type == kRequestTypePut))
    {
        if (urlArguments.size() == 2 && urlArguments[1] == "entities" && type == kRequestTypePost)
        {
            std::string record = urlArguments[0];
            replied = OnCreateEntity(connection, atoi(record.c_str()), queryArguments["type"]);
        } else if (urlArguments.size() == 4 && urlArguments[1] == "entity" && urlArguments[3] == "transform" && type == kRequestTypePut)
        {
            std::string record = urlArguments[0];
            std::string entity = urlArguments[2];
            replied = OnSetTransform(connection, atoi(record.c_str()), atoi(entity.c_str()), Vec2(atof(queryArguments["tx"].c_str()), atof(queryArguments["ty"].c_str())), 0, Vec2(1,1));
        }
    } else if (command == "records" && type == kRequestTypePost)
    {
        if (urlArguments.size() == 0)
        {
            replied = OnCreateRecord(connection, queryArguments["name"], queryArguments["type"]);
        }
    } else if (command == "save" && type == kRequestTypePut)
    {
        replied = OnSave(connection);
    } else if (command == "export" && type == kRequestTypePut)
    {
        replied = OnExport(connection);
    }
    
    if (replied == true)
    {
        connection.Send();
        return true;
    }
    
    if (type == kRequestTypeGet)
        return false; // No over-ride, default to finding html
    else
        return true;
}

bool HttpInterface::OnSave(pixelboost::HttpConnection& connection)
{
    Core::Instance()->GetCommandManager()->Exec("save");
    
    return true;
}

bool HttpInterface::OnExport(pixelboost::HttpConnection& connection)
{
    Core::Instance()->GetCommandManager()->Exec("export");
    
    return true;
}

bool HttpInterface::OnCreateRecord(pixelboost::HttpConnection& connection, const std::string& name, const std::string& type)
{
    if (name == "")
        return false;
    
    Project* project = Core::Instance()->GetProject();
    
    project->AddRecord(name, type);
    
    return true;
}

bool HttpInterface::OnCreateEntity(pixelboost::HttpConnection& connection, Uid recordId, const std::string& type)
{
    char arguments[256];
    sprintf(arguments, "-r %d -t %s -p 0,0", recordId, type.c_str());
    Core::Instance()->GetCommandManager()->Exec("createEntity", arguments);
    return true;
}

bool HttpInterface::OnSetTransform(pixelboost::HttpConnection& connection, Uid recordId, Uid entityId, const Vec2& position, float rotation, const Vec2& scale)
{
    Project* project = Core::Instance()->GetProject();
    
    Record* record = project->GetRecord(recordId);
    
    if (!record)
        return false;
    
    Entity* entity = record->GetEntity(entityId);
    
    if (!entity)
        return false;
    
    entity->SetPosition(position);
    entity->SetRotation(rotation);
    entity->SetScale(scale);
    
    return true;
}

bool HttpInterface::OnGetRecords(pixelboost::HttpConnection& connection)
{
    json::Array records;
    
    Project* project = Core::Instance()->GetProject();
    for (Project::RecordMap::const_iterator it = project->GetRecords().begin(); it != project->GetRecords().end(); ++it)
    {
        json::Object record;
        record["id"] = json::Number(it->first);
        record["name"] = json::String(it->second->GetName());
        record["type"] = json::String(it->second->GetTypeName());
        records.Insert(record);
    }
    
    std::stringstream contentStream;
    json::Writer::Write(records, contentStream);
    
    std::string content = contentStream.str();
    
    char contentLength[64];
    sprintf(contentLength, "%d", static_cast<int>(content.length()));
    connection.AddHeader("Content-Length", contentLength);
    connection.SetContent(content);
    
    return true;
}

bool HttpInterface::OnGetRecord(pixelboost::HttpConnection& connection, Uid recordId)
{
    json::Object data;
    
    Project* project = Core::Instance()->GetProject();
    
    Record* record = project->GetRecord(recordId);
    
    if (!record)
        return false;
    
    record->ExportJson(data);
    
    std::stringstream contentStream;
    json::Writer::Write(data, contentStream);
    
    std::string content = contentStream.str();
    
    char contentLength[64];
    sprintf(contentLength, "%d", static_cast<int>(content.length()));
    connection.AddHeader("Content-Length", contentLength);
    connection.SetContent(content);
    
    return true;
}

bool HttpInterface::OnGetEntity(pixelboost::HttpConnection& connection, Uid recordId, Uid entityId)
{
    json::Object data;
    
    Project* project = Core::Instance()->GetProject();
    
    Record* record = project->GetRecord(recordId);
    
    if (!record)
        return false;
    
    Entity* entity = record->GetEntity(entityId);
    
    if (!entity)
        return false;
    
    entity->ExportJson(data);
    
    std::stringstream contentStream;
    json::Writer::Write(data, contentStream);
    
    std::string content = contentStream.str();
    
    char contentLength[64];
    sprintf(contentLength, "%d", static_cast<int>(content.length()));
    connection.AddHeader("Content-Length", contentLength);
    connection.SetContent(content);
    
    return true;
}

bool HttpInterface::OnGetSchema(pixelboost::HttpConnection& connection)
{
    json::Object schema;
    
    Project* project = Core::Instance()->GetProject();
    
    json::Array structs;
    for (Schema::StructMap::const_iterator it = project->GetSchema()->GetStructs().begin(); it != project->GetSchema()->GetStructs().end(); ++it)
    {
        InsertSchemaItem(structs, it->second);
    }
    schema["structs"] = structs;
    
    std::stringstream contentStream;
    json::Writer::Write(schema, contentStream);
    
    std::string content = contentStream.str();
    
    char contentLength[64];
    sprintf(contentLength, "%d", static_cast<int>(content.length()));
    connection.AddHeader("Content-Length", contentLength);
    connection.SetContent(content);
    
    return true;
}

bool HttpInterface::OnGetImage(pixelboost::HttpConnection& connection, const std::string& image)
{
    Project* project = Core::Instance()->GetProject();

    const Project::ProjectConfig& config = project->GetConfig();
  
    std::fstream file;
    
    for (std::vector<std::string>::const_iterator it = config.imageRoots.begin(); it != config.imageRoots.end(); ++it)
    {
        file.open((*it + image).c_str());
        
        if (file.is_open())
            break;
    }
    
    if (file.is_open())
    {
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        char contentLength[64];
        sprintf(contentLength, "%d", static_cast<int>(content.length()));
        connection.AddHeader("Content-Length", contentLength);
        connection.SetContent(content);
        
        return true;
    }
    
    return false;
}

void HttpInterface::InsertSchemaItem(json::Array& array, SchemaStruct* schemaItem)
{
    json::Object item;
    item["name"] = json::String(schemaItem->GetName());
    const SchemaAttribute* description = schemaItem->GetAttribute("Description");
    item["description"] = json::String(description ? description->GetParamValue("") : schemaItem->GetName());
    
    json::Object visualisation;
    const SchemaAttribute* visualisationAttr = schemaItem->GetAttribute("Visualisation");
    if (visualisationAttr)
    {
        for (SchemaAttribute::ParamValueMap::const_iterator it = visualisationAttr->GetParamValues().begin(); it != visualisationAttr->GetParamValues().end(); ++it)
        {
            visualisation[it->first] = json::String(it->second);
        }
    }
    item["visualisation"] = visualisation;
    
    json::Array properties;
    for (SchemaStruct::PropertyMap::const_iterator it = schemaItem->GetProperties().begin(); it != schemaItem->GetProperties().end(); ++it)
    {
        json::Object property;
        ExportProperty(property, it->first, it->second);
        properties.Insert(property);
    }
    item["properties"] = properties;
    
    switch (schemaItem->GetSchemaType())
    {
        case SchemaItem::kSchemaRecord:
            item["type"] = json::String("record");
            break;
            
        case SchemaItem::kSchemaEntity:
            item["type"] = json::String("entity");
            break;

        case SchemaItem::kSchemaStruct:
            item["type"] = json::String("struct");
            break;
            
        default:
            item["type"] = json::String("invalid");
    }
    array.Insert(item);
}

void HttpInterface::ExportProperty(json::Object& property, const std::string& name, const SchemaProperty* schemaProp)
{
    if (name != "")
        property["name"] = json::String(name);
    
    switch (schemaProp->GetPropertyType())
    {
        case SchemaProperty::kSchemaPropertyAtom:
        {
            property["type"] = json::String("atom");
            switch (static_cast<const SchemaPropertyAtom*>(schemaProp)->GetAtomType())
            {
                case SchemaPropertyAtom::kSchemaAtomFloat:
                    property["atomType"] = json::String("float");
                    break;
                case SchemaPropertyAtom::kSchemaAtomInt:
                    property["atomType"] = json::String("int");
                    break;
                case SchemaPropertyAtom::kSchemaAtomString:
                    property["atomType"] = json::String("string");
                    break;
            }
            break;
        }
        case SchemaProperty::kSchemaPropertyArray:
        {
            property["type"] = json::String("array");
            const SchemaPropertyArray* propertyArray = static_cast<const SchemaPropertyArray*>(schemaProp);
            const SchemaProperty* schemaProperty = propertyArray->GetSchemaProperty();
            if (schemaProperty)
            {
                json::Object arrayType;
                ExportProperty(arrayType, "", schemaProperty);
                property["arrayType"] = arrayType;
            }
            break;
        }
        case SchemaProperty::kSchemaPropertyStruct:
        {
            property["type"] = json::String("struct");
            const SchemaStruct* propertyStruct = static_cast<const SchemaPropertyStruct*>(schemaProp)->GetSchemaStruct();
            if (propertyStruct)
            {
                property["structType"] = json::String(propertyStruct->GetName());
            }
            break;
        }
        case SchemaProperty::kSchemaPropertyPointer:
        {
            property["type"] = json::String("pointer");
            const SchemaEntity* propertyEntity = static_cast<const SchemaPropertyPointer*>(schemaProp)->GetSchemaEntity();
            if (propertyEntity)
            {
                property["pointerType"] = json::String(propertyEntity->GetName());
            }
            break;
        }
    }
}
