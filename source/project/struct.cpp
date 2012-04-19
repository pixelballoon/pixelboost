#include <stdio.h>

#include "project/struct.h"
#include "project/project.h"
#include "project/property.h"
#include "project/record.h"
#include "project/schema.h"
#include "project/struct.h"

#include "pixelboost/data/json/writer.h"

using namespace pixeleditor;

Struct::Struct(Project* project, const SchemaStruct* type)
    : _Uid(0)
    , _Name("")
    , _Type(type)
    , _Project(project)
{
    _Uid = _Project->CalculateUid();
    _TypeName = _Type ? _Type->GetName() : "";
}

Struct::~Struct()
{
    for (PropertyMap::iterator it = _Properties.begin(); it != _Properties.end(); ++it)
    {
        delete it->second;
    }
    _Properties.clear();
    
    _Project->ReleaseUid(_Uid);
}

bool Struct::Open(json::Object& entity)
{
    bool status = true;
    
    // Uid
    
    if (_Uid)
        _Project->ReleaseUid(_Uid);
    
    json::Number& id = entity["Uid"];
    _Uid = id.Value();
    
    if (_Uid)
        _Project->RegisterUid(_Uid);
    else
        status = false;
    
    // Type
    
    json::String& type = entity["Type"];
    _TypeName = type.Value();
    _Type = _Project->GetSchema()->GetStructByName(_TypeName);
    
    if (!_Type)
        status = false;
    
    // Name
    
    json::String& name = entity["Name"];
    _Name = name.Value();
    
    // Properties
    
    json::Object& properties = entity["Properties"];
    
    ParseProperties(properties, "");
    
    return status;
}

bool Struct::Save(json::Object& entity)
{
    entity["Type"] = json::String(_TypeName);
    entity["Name"] = json::String(_Name);
    entity["Uid"] = json::Number(_Uid);
    
    json::Object properties;
    
    for (PropertyMap::const_iterator it = _Properties.begin(); it != _Properties.end(); ++it)
    {
        std::string path = it->first;
        
        json::Object& container = GetPropertyContainer(path, properties);
        
        switch (it->second->GetType())
        {
            case Property::kPropertyAtom:
                container["_atom"] = json::String(static_cast<PropertyAtom*>(it->second)->GetStringValue());
                break;
                
            case Property::kPropertyPointer:
                container["_pointer"] = json::Number(static_cast<PropertyPointer*>(it->second)->GetPointerValue());
                break;
                
            case Property::kPropertyArray:
                PropertyArray* propArray = static_cast<PropertyArray*>(it->second);
                
                container["_array"] = json::Array();
                json::Array& array = container["_array"];
                for (int i = 0; i < propArray->GetElementCount(); ++i)
                {
                    char buffer[255];
                    sprintf(buffer, "#%X", propArray->GetElementIdByIndex(i));
                    array.Insert(json::String(buffer));
                }
                break;
        }
    }
    
    entity["Properties"] = properties;
    
    return true;
}
    
bool ExportProperty(Struct* s, const std::string& path, SchemaProperty* schemaItem, json::Object& container);

bool Struct::Export(json::Object& entity)
{
    entity["Type"] = json::String(_TypeName);
    entity["Uid"] = json::Number(_Uid);
    
    json::Object properties;
    
    for (SchemaStruct::PropertyMap::const_iterator it = _Type->GetProperties().begin(); it != _Type->GetProperties().end(); ++it)
    {
        ExportProperty(this, "/", it->second, properties);
    }
               
    entity["Properties"] = properties;
    
    return true;
}
    
json::UnknownElement ExportAtom(const PropertyAtom* atom, const SchemaPropertyAtom* schemaAtom)
{
    switch (schemaAtom->GetAtomType())
    {
        case SchemaPropertyAtom::kSchemaAtomFloat:
            return json::Number(atom->GetFloatValue());
            
        case SchemaPropertyAtom::kSchemaAtomInt:
            return json::Number(atom->GetIntValue());
            
        case SchemaPropertyAtom::kSchemaAtomString:
            return json::String(atom->GetStringValue());
    }
    
    // TODO : Assert here
    
    return json::UnknownElement();
}
    
json::UnknownElement ExportPointer(const PropertyPointer* pointer, const SchemaPropertyPointer* schemaPointer)
{
    return json::Number(pointer->GetPointerValue());
}
    
json::Object ExportStruct(Struct* s, const std::string& path, const SchemaStruct* schemaStruct)
{
    json::Object container;
    
    for (SchemaStruct::PropertyMap::const_iterator it = schemaStruct->GetProperties().begin(); it != schemaStruct->GetProperties().end(); ++it)
    {
        ExportProperty(s, path, it->second, container);
    }
    
    return container;
}
    
bool ExportProperty(Struct* s, const std::string& path, SchemaProperty* schemaItem, json::Object& container)
{
    std::string propertyPath = path + schemaItem->GetName() + "/";
    const Property* prop = s->GetProperty(propertyPath);
    
    switch (schemaItem->GetPropertyType())
    {
        case SchemaProperty::kSchemaPropertyAtom:
        {
            if (!prop || prop->GetType() != Property::kPropertyAtom)
                return true;
            
            SchemaPropertyAtom* schemaAtom = static_cast<SchemaPropertyAtom*>(schemaItem);
            const PropertyAtom* atom = static_cast<const PropertyAtom*>(prop);
            
            container[schemaItem->GetName()] = ExportAtom(atom, schemaAtom);
        }
            
        case SchemaProperty::kSchemaPropertyArray:
        {
            if (!prop || prop->GetType() != Property::kPropertyArray)
                return true;
            
            SchemaPropertyArray* schemaArray = static_cast<SchemaPropertyArray*>(schemaItem);
            const PropertyArray* array = static_cast<const PropertyArray*>(prop);
            
            const SchemaPropertyStruct* schemaStruct = schemaArray->GetSchemaStruct();
            
            container[schemaItem->GetName()] = json::Array();
            json::Array& output = container[schemaItem->GetName()];
            
            char buffer[255];
            for (int i=0; i < array->GetElementCount(); i++)
            {
                // TODO : Support non-struct arrays
                
                sprintf(buffer, "#%X", array->GetElementIdByIndex(i));
                
                output.Insert(ExportStruct(s, propertyPath + buffer + "/", schemaStruct->GetSchemaStruct()));
            }
            
            break;
        }
            
        case SchemaProperty::kSchemaPropertyStruct:
        {
            const SchemaPropertyStruct* schemaStruct = static_cast<SchemaPropertyStruct*>(schemaItem);
            
            container[schemaItem->GetName()] = ExportStruct(s, propertyPath, schemaStruct->GetSchemaStruct());
            
            break;
        }
            
        case SchemaProperty::kSchemaPropertyPointer:
        {
            if (!prop || prop->GetType() != Property::kPropertyPointer)
                return true;
            
            SchemaPropertyPointer* schemaPointer = static_cast<SchemaPropertyPointer*>(schemaItem);
            const PropertyPointer* pointer = static_cast<const PropertyPointer*>(prop);

            container[schemaItem->GetName()] = ExportPointer(pointer, schemaPointer);
            
            break;
        }
    }
    
    return true;
}

Project* Struct::GetProject()
{
    return _Project;
}

const std::string& Struct::GetTypeName() const
{
    return _TypeName;
}

const SchemaStruct* Struct::GetType() const
{
    return _Type;
}

void Struct::SetUid(Uid uid)
{
    _Project->ReleaseUid(_Uid);
    _Uid = uid;
    _Project->RegisterUid(_Uid);
}

Uid Struct::GetUid() const
{
    return _Uid;
}

const std::string& Struct::GetName() const
{
    return _Name;
}

void Struct::SetName(const std::string& name)
{
    _Name = name;
}

const Property* Struct::GetProperty(const std::string& path) const
{
    PropertyMap::const_iterator it = _Properties.find(path);
    
    if (it != _Properties.end())
        return it->second;
    
    return 0;
}

PropertyAtom* Struct::AcquireAtom(const std::string& path)
{
    PropertyMap::const_iterator it = _Properties.find(path);
    
    if (it != _Properties.end())
    {
        if (it->second->GetType() == Property::kPropertyAtom)
            return static_cast<PropertyAtom*>(it->second);
        else
            delete it->second;
    }
    
    const SchemaProperty* schemaProperty = _Type->FindPropertyByPath(path);
    PropertyAtom* property = new PropertyAtom(this, schemaProperty);
    
    _Properties[path] = property;
    
    return property;
}

PropertyPointer* Struct::AcquirePointer(const std::string& path)
{
    PropertyMap::const_iterator it = _Properties.find(path);
    
    if (it != _Properties.end())
    {
        if (it->second->GetType() == Property::kPropertyPointer)
            return static_cast<PropertyPointer*>(it->second);
        else
            delete it->second;
    }
    
    const SchemaProperty* schemaProperty = _Type->FindPropertyByPath(path);
    PropertyPointer* property = new PropertyPointer(this, schemaProperty);
    
    _Properties[path] = property;
    
    return property;
}

PropertyArray* Struct::AcquireArray(const std::string& path)
{
    PropertyMap::const_iterator it = _Properties.find(path);
    
    if (it != _Properties.end())
    {
        if (it->second->GetType() == Property::kPropertyArray)
            return static_cast<PropertyArray*>(it->second);
        else
            delete it->second;
    }
    
    const SchemaProperty* schemaProperty = _Type->FindPropertyByPath(path);
    PropertyArray* property = new PropertyArray(this, schemaProperty);
    
    _Properties[path] = property;
    
    return property;
}

bool Struct::ParseProperties(json::Object& container, std::string path)
{
    for (json::Object::iterator it = container.Begin(); it != container.End(); ++it)
    {
        std::string name = it->name;
        
        if (name.length() == 0)
            continue;
        
        if (name[0] == '_')
        {
            Property* property = 0;
            
            if (name == "_atom")
            {
                PropertyAtom* atom = new PropertyAtom(this, _Type ? _Type->FindPropertyByPath(path) : 0);
                json::String& value = it->element;
                atom->SetStringValue(value.Value());
                property = atom;
            }
            else if (name == "_pointer")
            {
                PropertyPointer* pointer = new PropertyPointer(this, _Type ? _Type->FindPropertyByPath(path) : 0);
                json::Number& value = it->element;
                pointer->SetPointerValue(value.Value());
                property = pointer;
            }
            else if (name == "_array")
            {
                PropertyArray* array = new PropertyArray(this, _Type ? _Type->FindPropertyByPath(path) : 0);
                json::Array& value = it->element;
                for (json::Array::iterator arrayIt = value.Begin(); arrayIt != value.End(); ++arrayIt)
                {
                    json::String& item = *arrayIt;
                    array->AddElement(strtoul(item.Value().substr(1, std::string::npos).c_str(), NULL, 16));
                }
                property = array;
            }
            
            if (property)
                _Properties[path + "/"] = property;
            
            continue;
        }
        
        json::Object& object = it->element;
        
        ParseProperties(object, path + "/" + name);
    }
    
    return true;
}

json::Object& Struct::GetPropertyContainer(std::string path, json::Object& parent)
{
    size_t seperator = path.find('/');
    if (seperator == std::string::npos)
        return parent;
    
    if (seperator == 0)
        return GetPropertyContainer(path.substr(1, std::string::npos), parent);
    
    std::string element = path.substr(0, seperator);
    std::string remaining = path.substr(seperator+1, std::string::npos);
    
    if (parent.Find(element) == parent.End())
        parent[element] = json::Object();
    
    return GetPropertyContainer(remaining, parent[element]);
}
