#include <sstream>

#include "pixelboost/data/json/writer.h"

#include "http/commands/schema.h"
#include "http/httpInterface.h"
#include "project/project.h"
#include "project/schema.h"
#include "core.h"

using namespace pixeleditor;

SchemaCommands::SchemaCommands()
{
    Core::Instance()->GetHttpInterface()->RegisterCommand("schema", pb::HttpServer::kRequestTypeGet, HttpInterface::CommandDelegate::from_method<SchemaCommands, &SchemaCommands::OnGetSchema>(this));
}

bool SchemaCommands::OnGetSchema(pb::HttpConnection& connection, const std::vector<std::string>& urlArguments, const std::map<std::string, std::string>& queryArguments)
{
    json::Array schema;
    
    Project* project = Core::Instance()->GetProject();
    
    for (Schema::StructMap::const_iterator it = project->GetSchema()->GetStructs().begin(); it != project->GetSchema()->GetStructs().end(); ++it)
    {
        InsertSchemaItem(schema, it->second);
    }
    
    std::stringstream contentStream;
    json::Writer::Write(schema, contentStream);
    
    std::string content = contentStream.str();
    
    char contentLength[64];
    sprintf(contentLength, "%d", static_cast<int>(content.length()));
    connection.AddHeader("Content-Length", contentLength);
    connection.SetContent(content);
    
    return true;
}

void SchemaCommands::InsertSchemaItem(json::Array& array, SchemaStruct* schemaItem)
{
    json::Object item;
    item["name"] = json::String(schemaItem->GetName());
    
    ExportAttributes(item, schemaItem);
    
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

void SchemaCommands::ExportAttributes(json::Object& parent, const SchemaItem* schemaItem)
{
    json::Object attributes;
    for (SchemaItem::AttributeMap::const_iterator it = schemaItem->GetAttributes().begin(); it != schemaItem->GetAttributes().end(); ++it)
    {
        json::Object attribute;
        
        for (SchemaAttribute::ParamValueMap::const_iterator paramIt = it->second->GetParamValues().begin(); paramIt != it->second->GetParamValues().end(); ++paramIt)
        {
            attribute[paramIt->first] = json::String(paramIt->second);
        }
        
        attributes[it->first] = attribute;
    }
    parent["attributes"] = attributes;
}

void SchemaCommands::ExportProperty(json::Object& property, const std::string& name, const SchemaProperty* schemaProp)
{
    if (name != "")
        property["name"] = json::String(name);
    
    ExportAttributes(property, schemaProp);
    
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
