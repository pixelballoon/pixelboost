#include <sstream>
#include <stdio.h>

#include "project/struct.h"
#include "project/project.h"
#include "project/property.h"
#include "project/record.h"
#include "project/schema.h"
#include "project/struct.h"

#include "pixelboost/data/json/writer.h"
#include "pixelboost/framework/definitions.h"

using namespace pixeleditor;

ProjectStruct::ProjectStruct(Project* project, const SchemaStruct* type)
    : _Uid(0)
    , _Name("")
    , _Type(type)
    , _Project(project)
{
    _Uid = _Project->CalculateUid();
    _TypeName = _Type ? _Type->GetName() : "";
}

ProjectStruct::~ProjectStruct()
{
    destroyed(this);
    
    for (PropertyMap::iterator it = _Properties.begin(); it != _Properties.end(); ++it)
    {
        delete it->second;
    }
    _Properties.clear();
    
    _Project->ReleaseUid(_Uid);
}

bool ProjectStruct::Open(json::Object& entity, bool skipUid)
{
    bool status = true;
    
    // Uid
    
    if (!skipUid)
    {
        if (_Uid)
            _Project->ReleaseUid(_Uid);
        
        json::Number& id = entity["Uid"];
        _Uid = id.Value();
        
        if (_Uid)
            _Project->RegisterUid(_Uid);
        else
            status = false;
    }
    
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

bool ProjectStruct::Save(json::Object& entity)
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
                
            case Property::kPropertyReference:
                container["_reference"] = json::Number(static_cast<PropertyReference*>(it->second)->GetReferenceValue());
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

class JsonExporter
{
public:
    static bool ExportProperty(ProjectStruct* s, const std::string& path, const SchemaProperty* schemaItem, json::Object& container, bool appendPath = true);
    
private:
    static json::UnknownElement ExportAtom(const PropertyAtom* atom, const SchemaPropertyAtom* schemaAtom);
    static json::UnknownElement ExportPointer(const PropertyPointer* pointer, const SchemaPropertyPointer* schemaPointer);
    static json::UnknownElement ExportReference(const PropertyReference* reference, const SchemaPropertyReference* schemaReference);
    static json::Object ExportStruct(ProjectStruct* s, const std::string& path, const SchemaStruct* schemaStruct);    
};

class LuaExporter
{
public:
    static bool ExportProperty(std::iostream& output, ProjectStruct* s, const std::string& path, const SchemaProperty* schemaItem, bool appendPath = true);
    
private:
    static bool ExportAtom(std::iostream& output, const PropertyAtom* atom, const SchemaPropertyAtom* schemaAtom);
    static bool ExportPointer(std::iostream& output, const PropertyPointer* pointer, const SchemaPropertyPointer* schemaPointer);
    static bool ExportReference(std::iostream& output, const PropertyReference* reference, const SchemaPropertyReference* schemaReference);
    static bool ExportStruct(std::iostream& output, ProjectStruct* s, const std::string& path, const SchemaStruct* schemaStruct);    
};

bool ProjectStruct::ExportJson(json::Object& entity)
{
    entity["Type"] = json::String(_TypeName);
    entity["Uid"] = json::Number(_Uid);
    
    json::Object properties;
    
	ExportJsonProperties(properties, _Type);
               
    entity["Properties"] = properties;
    
    return true;
}

void ProjectStruct::ExportJsonProperties(json::Object& properties, const SchemaStruct* type)
{
	if (!type)
		return;
	
	for (SchemaStruct::PropertyList::const_iterator it = type->GetProperties().begin(); it != type->GetProperties().end(); ++it)
	{
		JsonExporter::ExportProperty(this, "/", it->second, properties);
	}

	ExportJsonProperties(properties, type->GetBaseType());
}

Uid ProjectStruct::GetTypeHash() const
{
    return pb::TypeHash(_TypeName.c_str());
}

bool ProjectStruct::ExportLua(std::iostream& output, bool appendNewLine)
{
    output << "type = " << GetTypeHash() << "," << std::endl;
    output << "uid = " << _Uid << "," << std::endl;
    
    output << "properties = {" << std::endl;
    
	ExportLuaProperties(output, _Type, false);

    output << "}";
    
    if (appendNewLine)
        output << std::endl;
    
    return false;
}

void ProjectStruct::ExportLuaProperties(std::iostream& output, const SchemaStruct* type, bool appendComma)
{
	if (!type)
		return;
	
	for (SchemaStruct::PropertyList::const_iterator it = type->GetProperties().begin(); it != type->GetProperties().end(); ++it)
	{
		std::stringstream property;
		bool status = LuaExporter::ExportProperty(property, this, "/", it->second);
		
		if (status)
		{
			if (appendComma)
			{
                output << ",";
			}
			
			output << property.str();
			appendComma = true;
		}
	}
	
	ExportLuaProperties(output, type->GetBaseType(), appendComma);
}

json::UnknownElement JsonExporter::ExportAtom(const PropertyAtom* atom, const SchemaPropertyAtom* schemaAtom)
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
    
json::UnknownElement JsonExporter::ExportPointer(const PropertyPointer* pointer, const SchemaPropertyPointer* schemaPointer)
{
    return json::Number(pointer->GetPointerValue());
}

json::UnknownElement JsonExporter::ExportReference(const PropertyReference* reference, const SchemaPropertyReference* schemaReference)
{
    return json::Number(reference->GetReferenceValue());
}
    
json::Object JsonExporter::ExportStruct(ProjectStruct* s, const std::string& path, const SchemaStruct* schemaStruct)
{
    json::Object container;
    
    for (SchemaStruct::PropertyList::const_iterator it = schemaStruct->GetProperties().begin(); it != schemaStruct->GetProperties().end(); ++it)
    {
        ExportProperty(s, path, it->second, container);
    }
    
    return container;
}
    
bool JsonExporter::ExportProperty(ProjectStruct* s, const std::string& path, const SchemaProperty* schemaItem, json::Object& container, bool appendPath)
{
    std::string propertyPath = path;
    
    if (appendPath)
        propertyPath += schemaItem->GetName() + "/";
    
    const Property* prop = s->GetProperty(propertyPath);
    
    switch (schemaItem->GetPropertyType())
    {
        case SchemaProperty::kSchemaPropertyAtom:
        {
            if (!prop || prop->GetType() != Property::kPropertyAtom)
                return true;
            
            const SchemaPropertyAtom* schemaAtom = static_cast<const SchemaPropertyAtom*>(schemaItem);
            const PropertyAtom* atom = static_cast<const PropertyAtom*>(prop);
            
            container[schemaItem->GetName()] = ExportAtom(atom, schemaAtom);
            
            break;
        }
            
        case SchemaProperty::kSchemaPropertyArray:
        {
            if (!prop || prop->GetType() != Property::kPropertyArray)
                return true;
            
            const SchemaPropertyArray* schemaArray = static_cast<const SchemaPropertyArray*>(schemaItem);
            const PropertyArray* array = static_cast<const PropertyArray*>(prop);
            
            const SchemaProperty* schemaProperty = schemaArray->GetSchemaProperty();
            
            container[schemaItem->GetName()] = json::Array();
            json::Array& output = container[schemaItem->GetName()];
            
            char buffer[255];
            for (int i=0; i < array->GetElementCount(); i++)
            {
                sprintf(buffer, "#%X", array->GetElementIdByIndex(i));
                
                json::Object item;
                ExportProperty(s, propertyPath + buffer + "/", schemaProperty, container, false);
                output.Insert(item);
            }
            break;
        }
            
        case SchemaProperty::kSchemaPropertyStruct:
        {
            const SchemaPropertyStruct* schemaStruct = static_cast<const SchemaPropertyStruct*>(schemaItem);
            
            container[schemaItem->GetName()] = ExportStruct(s, propertyPath, schemaStruct->GetSchemaStruct());
            
            break;
        }
            
        case SchemaProperty::kSchemaPropertyPointer:
        {
            if (!prop || prop->GetType() != Property::kPropertyPointer)
                return true;
            
            const SchemaPropertyPointer* schemaPointer = static_cast<const SchemaPropertyPointer*>(schemaItem);
            const PropertyPointer* pointer = static_cast<const PropertyPointer*>(prop);

            container[schemaItem->GetName()] = ExportPointer(pointer, schemaPointer);
            
            break;
        }
            
        case SchemaProperty::kSchemaPropertyReference:
        {
            if (!prop || prop->GetType() != Property::kPropertyReference)
                return true;
            
            const SchemaPropertyReference* schemaReference = static_cast<const SchemaPropertyReference*>(schemaItem);
            const PropertyReference* reference = static_cast<const PropertyReference*>(prop);
            
            container[schemaItem->GetName()] = ExportReference(reference, schemaReference);
            
            break;
        }
    }
    
    return true;
}

bool LuaExporter::ExportAtom(std::iostream& output, const PropertyAtom* atom, const SchemaPropertyAtom* schemaAtom)
{
    switch (schemaAtom->GetAtomType())
    {
        case SchemaPropertyAtom::kSchemaAtomFloat:
            output << atom->GetFloatValue();
            break;
            
        case SchemaPropertyAtom::kSchemaAtomInt:
            output << atom->GetIntValue();
            break;
            
        case SchemaPropertyAtom::kSchemaAtomString:
            output << "\"" << atom->GetStringValue() << "\"";
            break;
    }
    
    return true;
}

bool LuaExporter::ExportPointer(std::iostream& output, const PropertyPointer* pointer, const SchemaPropertyPointer* schemaPointer)
{
    output << pointer->GetPointerValue();
    return true;
}

bool LuaExporter::ExportReference(std::iostream& output, const PropertyReference* reference, const SchemaPropertyReference* schemaReference)
{
    output << reference->GetReferenceValue();
    return true;
}

bool LuaExporter::ExportStruct(std::iostream& output, ProjectStruct* s, const std::string& path, const SchemaStruct* schemaStruct)
{
    output << "{";
    
    bool appendComma = false;
    for (SchemaStruct::PropertyList::const_iterator it = schemaStruct->GetProperties().begin(); it != schemaStruct->GetProperties().end(); ++it)
    {   
        std::stringstream property;
        bool status = ExportProperty(property, s, path, it->second, true);
        
        if (status)
        {
            if (appendComma)
            {
                output << ",";
            }
            
            output << property.str();
            appendComma = true;
        }
    }
    
    output << "}";
    
    return true;
}

bool LuaExporter::ExportProperty(std::iostream& output, ProjectStruct* s, const std::string& path, const SchemaProperty* schemaItem, bool appendPath)
{
    std::string propertyPath = path;
    
    if (appendPath)
        propertyPath += schemaItem->GetName() + "/";
    
    const Property* prop = s->GetProperty(propertyPath);
    
    bool status = false;
    
    switch (schemaItem->GetPropertyType())
    {
        case SchemaProperty::kSchemaPropertyAtom:
        {
            if (!prop || prop->GetType() != Property::kPropertyAtom)
                return false;
            
            if (appendPath)
                output << schemaItem->GetName() << " = ";
            
            const SchemaPropertyAtom* schemaAtom = static_cast<const SchemaPropertyAtom*>(schemaItem);
            const PropertyAtom* atom = static_cast<const PropertyAtom*>(prop);
            
            status = ExportAtom(output, atom, schemaAtom);
            
            break;
        }
            
        case SchemaProperty::kSchemaPropertyArray:
        {
            if (!prop || prop->GetType() != Property::kPropertyArray)
                return false;
            
            if (appendPath)
                output << schemaItem->GetName() << " = ";
            
            const SchemaPropertyArray* schemaArray = static_cast<const SchemaPropertyArray*>(schemaItem);
            const PropertyArray* array = static_cast<const PropertyArray*>(prop);
            
            const SchemaProperty* schemaProperty = schemaArray->GetSchemaProperty();
            
            status = true;
            
            output << "{";
            
            char buffer[255];
            for (int i=0; i < array->GetElementCount();)
            {
                sprintf(buffer, "#%X", array->GetElementIdByIndex(i));
                
                if (!ExportProperty(output, s, propertyPath + buffer + "/", schemaProperty, false))
                    status = false;
                
                ++i;
                
                if (i < array->GetElementCount())
                    output << ",";
            }
            
            output << "}";
            
            break;
        }
            
        case SchemaProperty::kSchemaPropertyStruct:
        {
            if (appendPath)
                output << schemaItem->GetName() << " = ";

            const SchemaPropertyStruct* schemaStruct = static_cast<const SchemaPropertyStruct*>(schemaItem);
            
            status = ExportStruct(output, s, propertyPath, schemaStruct->GetSchemaStruct());
            
            break;
        }
            
        case SchemaProperty::kSchemaPropertyPointer:
        {
            if (!prop || prop->GetType() != Property::kPropertyPointer)
                return false;
            
            if (appendPath)
                output << schemaItem->GetName() << " = ";
            
            const SchemaPropertyPointer* schemaPointer = static_cast<const SchemaPropertyPointer*>(schemaItem);
            const PropertyPointer* pointer = static_cast<const PropertyPointer*>(prop);
            
            status = ExportPointer(output, pointer, schemaPointer);
            
            break;
        }
            
        case SchemaProperty::kSchemaPropertyReference:
        {
            if (!prop || prop->GetType() != Property::kPropertyReference)
                return false;
            
            if (appendPath)
                output << schemaItem->GetName() << " = ";
            
            const SchemaPropertyReference* schemaReference = static_cast<const SchemaPropertyReference*>(schemaItem);
            const PropertyReference* reference = static_cast<const PropertyReference*>(prop);
            
            status = ExportReference(output, reference, schemaReference);
            
            break;
        }
    }
    
    return status;
}

Project* ProjectStruct::GetProject()
{
    return _Project;
}

const std::string& ProjectStruct::GetTypeName() const
{
    return _TypeName;
}

const SchemaStruct* ProjectStruct::GetType() const
{
    return _Type;
}

void ProjectStruct::SetUid(Uid uid)
{
    _Project->ReleaseUid(_Uid);
    _Uid = uid;
    _Project->RegisterUid(_Uid);
}

Uid ProjectStruct::GetUid() const
{
    return _Uid;
}

const std::string& ProjectStruct::GetName() const
{
    return _Name;
}

void ProjectStruct::SetName(const std::string& name)
{
    _Name = name;
}

float ProjectStruct::GetPropertyFloat(const std::string& path, float defaultValue)
{
    const Property* property = GetProperty(path);
    
    if (!property || !property->AsAtom())
        return defaultValue;
    
    return property->AsAtom()->GetFloatValue();
}

int ProjectStruct::GetPropertyInt(const std::string& path, int defaultValue)
{
    const Property* property = GetProperty(path);
    
    if (!property || !property->AsAtom())
        return defaultValue;
    
    return property->AsAtom()->GetIntValue();
}

std::string ProjectStruct::GetPropertyString(const std::string& path, const std::string& defaultValue)
{
    const Property* property = GetProperty(path);
    
    if (!property || !property->AsAtom())
        return defaultValue;
    
    return property->AsAtom()->GetStringValue();
}

const Property* ProjectStruct::GetProperty(const std::string& path) const
{
    PropertyMap::const_iterator it = _Properties.find(path);
    
    if (it != _Properties.end())
        return it->second;
    
    return 0;
}

PropertyAtom* ProjectStruct::AcquireAtom(const std::string& path)
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

PropertyPointer* ProjectStruct::AcquirePointer(const std::string& path)
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

PropertyReference* ProjectStruct::AcquireReference(const std::string& path)
{
    PropertyMap::const_iterator it = _Properties.find(path);
    
    if (it != _Properties.end())
    {
        if (it->second->GetType() == Property::kPropertyReference)
            return static_cast<PropertyReference*>(it->second);
        else
            delete it->second;
    }
    
    const SchemaProperty* schemaProperty = _Type->FindPropertyByPath(path);
    PropertyReference* property = new PropertyReference(this, schemaProperty);
    
    _Properties[path] = property;
    
    return property;
}

PropertyArray* ProjectStruct::AcquireArray(const std::string& path)
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

bool ProjectStruct::ParseProperties(json::Object& container, std::string path)
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
            else if (name == "_reference")
            {
                PropertyReference* reference = new PropertyReference(this, _Type ? _Type->FindPropertyByPath(path) : 0);
                json::Number& value = it->element;
                reference->SetReferenceValue(value.Value());
                property = reference;
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

json::Object& ProjectStruct::GetPropertyContainer(std::string path, json::Object& parent)
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
