#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "project/schema.h"
#include "project/schemaParser.h"
#include "project/struct.h"

using namespace pixeleditor;
    
std::vector<std::string> &SplitString(const std::string &s, char delim, std::vector<std::string> &items)
{
    std::stringstream stream(s);
    std::string item;
    while(std::getline(stream, item, delim)) {
        items.push_back(item);
    }
    return items;
}

std::vector<std::string> SplitPath(const std::string &string)
{
    std::vector<std::string> items;
    return SplitString(string, '/', items);
}
    
SchemaAttribute::SchemaAttribute(const std::string& name)
    : _Name(name)
{
    
}
    
const std::string& SchemaAttribute::GetName() const
{
    return _Name;
}
    
void SchemaAttribute::AddParam(const std::string& param, const std::string& value)
{
    _ParamValue[param] = value;
}

std::string SchemaAttribute::EvaluateParamValue(ProjectStruct* structure, const std::string& param, const std::string& prefix, const std::string& defaultValue) const
{
    ParamValueMap::const_iterator it = _ParamValue.find(param);
    
    if (it == _ParamValue.end())
        return defaultValue;
    
    std::string paramValue = it->second;
    
    if (paramValue[0] == '/')
        return structure->EvaluateProperty(prefix+paramValue);
    
    return paramValue;
}

bool SchemaAttribute::HasParamValue(const std::string& param) const
{
    return (_ParamValue.find(param) != _ParamValue.end());
}

std::string SchemaAttribute::GetParamValue(const std::string& param, const std::string& defaultValue) const
{
    ParamValueMap::const_iterator it = _ParamValue.find(param);
    
    if (it != _ParamValue.end())
        return it->second;
    
    return defaultValue;
}

const SchemaAttribute::ParamValueMap& SchemaAttribute::GetParamValues() const
{
    return _ParamValue;
}

SchemaItem::SchemaItem(SchemaItem* parent, const std::string& name)
    : _Name(name)
    , _Parent(parent)
{
    if (parent != 0)
    {
        while (parent->GetSchemaType() != kSchema)
            parent = parent->_Parent;
        
        _Uid = static_cast<Schema*>(parent)->FindFreeUid(this);
    }
}

SchemaItem::~SchemaItem()
{
    for (AttributeMap::iterator it = _Attributes.begin(); it != _Attributes.end(); ++it)
        delete it->second;
    
    _Attributes.clear();
}
    
const Schema* SchemaItem::GetSchema() const
{
    const SchemaItem* item = this;
    
    while (item->_Parent)
        item = item->_Parent;
    
    return static_cast<const Schema*>(item);
}

SchemaItem* SchemaItem::GetParent()
{
    return _Parent;
}

const std::string& SchemaItem::GetName() const
{
    return _Name;
}

Uid SchemaItem::GetUid()
{
    return _Uid;
}

void SchemaItem::SetAttributes(const std::vector<SchemaAttribute*> &attributes)
{
    for (std::vector<SchemaAttribute*>::const_iterator it = attributes.begin(); it != attributes.end(); ++it)
    {
        _Attributes[(*it)->GetName()] = *it;
    }
}

const SchemaAttribute* SchemaItem::GetAttribute(std::string const &name) const
{
    AttributeMap::const_iterator it = _Attributes.find(name);
    if (it != _Attributes.end())
        return it->second;
    
    return 0;
}

const SchemaItem::AttributeMap& SchemaItem::GetAttributes() const
{
    return _Attributes;
}
    
SchemaStruct::SchemaStruct(Schema* schema, const std::string& name)
    : SchemaItem(schema, name)
    , _Base("")
    , _BaseCache(0)
{
}
    
SchemaStruct::~SchemaStruct()
{
    for (PropertyMap::iterator it = _Properties.begin(); it != _Properties.end(); ++it)
    {
        delete it->second;
    }
}

SchemaItem::SchemaType SchemaStruct::GetSchemaType() const
{
    return SchemaItem::kSchemaStruct;
}
    
void SchemaStruct::SetBaseType(std::string const &base)
{
    _Base = base;
    _BaseCache = 0;
}
    
const SchemaStruct* SchemaStruct::GetBaseType() const
{
    if (!_BaseCache)
        _BaseCache = GetSchema()->GetStructByName(_Base);
    
    return _BaseCache;
}

void SchemaStruct::AddProperty(SchemaProperty* property)
{
    // TODO: Check if a property of the same name already exists
    
    _Properties[property->GetName()] = property;
}

const SchemaStruct::PropertyMap& SchemaStruct::GetProperties() const
{
    return _Properties;
}

const SchemaProperty* SchemaStruct::GetPropertyByName(const std::string& name) const
{
    PropertyMap::const_iterator it = _Properties.find(name);
    
    if (it != _Properties.end())
        return it->second;
    
    return 0;
}

const SchemaProperty* SchemaStruct::FindPropertyByPath(const std::string& path) const
{
    std::vector<std::string> splitPath = SplitPath(path);
    
    const SchemaStruct* schemaStruct = this;
    
    for (std::vector<std::string>::iterator it = splitPath.begin(); it != splitPath.end(); ++it)
    {
        if (*it == "")
            continue;
        
        if (!schemaStruct)
            return 0;
        
        PropertyMap::const_iterator propertyIt = schemaStruct->_Properties.find(*it);
        
        if (propertyIt != schemaStruct->_Properties.end())
        {
            const SchemaProperty* property = propertyIt->second;
            
            if (property->GetPropertyType() == SchemaProperty::kSchemaPropertyArray)
            {
                property = static_cast<const SchemaPropertyArray*>(property)->GetSchemaProperty();
            }
            
            if (property->GetPropertyType() == SchemaProperty::kSchemaPropertyAtom ||
                property->GetPropertyType() == SchemaProperty::kSchemaPropertyPointer)
            {
                return property;
            }
            else if (property->GetPropertyType() == SchemaProperty::kSchemaPropertyStruct)
            {
                schemaStruct = static_cast<const SchemaPropertyStruct*>(property)->GetSchemaStruct();
            }
        } else {
            return 0;
        }
    }
    
    return 0;
}
    
SchemaRecord::SchemaRecord(Schema* schema, const std::string& name)
    : SchemaStruct(schema, name)
{
}
    
SchemaRecord::~SchemaRecord()
{
    
}
    
SchemaItem::SchemaType SchemaRecord::GetSchemaType() const
{
    return SchemaItem::kSchemaRecord;
}

SchemaEntity::SchemaEntity(Schema* schema, const std::string& name)
: SchemaStruct(schema, name)
{
}

SchemaEntity::~SchemaEntity()
{
    
}

SchemaItem::SchemaType SchemaEntity::GetSchemaType() const
{
    return SchemaItem::kSchemaEntity;
}

Schema::Schema()
    : SchemaItem(0, "")
    , _IsOpen(false)
{

}

Schema::~Schema()
{
    Close();
}

SchemaItem::SchemaType Schema::GetSchemaType() const
{
    return SchemaItem::kSchema;
}

bool Schema::Open(const std::string& directory)
{
    if (_IsOpen)
        Close();
    
    std::string schemaFile = directory + "main.txt";
    
    std::fstream file;
    
    file.open(schemaFile.c_str(), std::ios_base::in);
    
    if (!file.is_open())
        return false;
    
    std::string schema((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    
    file.close();
    
    bool result = SchemaParser::Parse(this, schema);
    
    if (result)
    {
        _IsOpen = true;
        schemaOpened(this);        
    }
    
    return result;
}

bool Schema::Close()
{
    if (!_IsOpen)
        return false;
    
    _IsOpen = false;
    
    for (StructMap::iterator it = _Structs.begin(); it != _Structs.end(); ++it)
    {
        delete it->second;
    }
    _Entities.clear();
    _Structs.clear();
    
    _UidMap.clear();
    
    schemaClosed(this);
    
    return true;
}

Uid Schema::FindFreeUid(SchemaItem* item)
{
    for (int i=0; i<128; i++)
    {
        Uid id = rand();
        UidMap::iterator it = _UidMap.find(id);
        if (it == _UidMap.end())
        {
            _UidMap[id] = item;
            return id;
        }
    }
    
    // TODO : Assert here!
    
    return 0;
}
    
bool Schema::AddStruct(SchemaStruct* schemaStruct)
{
    StructMap::iterator it = _Structs.find(schemaStruct->GetName());
    
    if (it == _Structs.end())
    {
        _Structs[schemaStruct->GetName()] = schemaStruct;
        
        return true;
    }
    
    // TODO : Warning here!
    
    return false;
}
    
bool Schema::AddRecord(SchemaRecord* schemaRecord)
{
    if (!AddStruct(schemaRecord))
        return false;
    
    RecordMap::iterator it = _Records.find(schemaRecord->GetName());
    
    if (it == _Records.end())
    {
        _Records[schemaRecord->GetName()] = schemaRecord;
        
        return true;
    }
    
    // TODO : Warning here!
    
    return false;
}

bool Schema::AddEntity(SchemaEntity* schemaEntity)
{
    if (!AddStruct(schemaEntity))
        return false;
    
    EntityMap::iterator it = _Entities.find(schemaEntity->GetName());
    
    if (it == _Entities.end())
    {
        _Entities[schemaEntity->GetName()] = schemaEntity;
        
        return true;
    }
    
    // TODO : Warning here!
    
    return false;
}

SchemaItem* Schema::GetItemByUid(Uid uid)
{
    UidMap::iterator it = _UidMap.find(uid);
    
    if (it != _UidMap.end())
        return it->second;
    
    return 0;
}
    
const SchemaStruct* Schema::GetStructByName(const std::string& name) const
{
    StructMap::const_iterator it = _Structs.find(name);
    
    if (it != _Structs.end())
        return it->second;
    
    return 0;
}

const SchemaRecord* Schema::GetRecordByName(const std::string& name) const
{
    RecordMap::const_iterator it = _Records.find(name);
    
    if (it != _Records.end())
        return it->second;
    
    return 0;
}

const SchemaEntity* Schema::GetEntityByName(const std::string& name) const
{
    EntityMap::const_iterator it = _Entities.find(name);
    
    if (it != _Entities.end())
        return it->second;
    
    return 0;
}
    
const Schema::StructMap& Schema::GetStructs() const
{
    return _Structs;
}

const Schema::RecordMap& Schema::GetRecords() const
{
    return _Records;
}

const Schema::EntityMap& Schema::GetEntities() const
{
    return _Entities;
}

SchemaProperty::SchemaProperty(SchemaItem* parent, const std::string& name)
: SchemaItem(parent, name)
{
}

SchemaProperty::~SchemaProperty()
{
    
}

SchemaItem::SchemaType SchemaProperty::GetSchemaType() const
{
    return SchemaItem::kSchemaProperty;
}

SchemaPropertyAtom::SchemaPropertyAtom(SchemaItem* parent, const std::string& type, const std::string& name)
    : SchemaProperty(parent, name)
{
    if (type == "float")
        _AtomType = kSchemaAtomFloat;
    else if (type == "int")
        _AtomType = kSchemaAtomInt;
    else if (type == "string")
        _AtomType = kSchemaAtomString;
}
    
SchemaPropertyAtom::~SchemaPropertyAtom()
{
    
}

SchemaProperty::SchemaPropertyType SchemaPropertyAtom::GetPropertyType() const
{
    return kSchemaPropertyAtom;
}
    
SchemaPropertyAtom::SchemaAtomType SchemaPropertyAtom::GetAtomType() const
{
    return _AtomType;
}

SchemaPropertyArray::SchemaPropertyArray(SchemaItem* parent, const std::string& type, const std::string& name, SchemaProperty* schemaProperty)
    : SchemaProperty(parent, name)
{
    _Property = schemaProperty;
}
    
SchemaPropertyArray::~SchemaPropertyArray()
{
    
}

SchemaProperty::SchemaPropertyType SchemaPropertyArray::GetPropertyType() const
{
    return kSchemaPropertyArray;    
}
    
const SchemaProperty* SchemaPropertyArray::GetSchemaProperty() const
{
    return _Property;
}

SchemaPropertyStruct::SchemaPropertyStruct(SchemaItem* parent, const std::string& type, const std::string& name)
    : SchemaProperty(parent, name)
    , _Type(type)
{

}
    
SchemaPropertyStruct::~SchemaPropertyStruct()
{
    
}

SchemaProperty::SchemaPropertyType SchemaPropertyStruct::GetPropertyType() const
{
    return kSchemaPropertyStruct;
}
    
const SchemaStruct* SchemaPropertyStruct::GetSchemaStruct() const
{
    const Schema* schema = GetSchema();
    return schema->GetStructByName(_Type);
}
    
SchemaPropertyPointer::SchemaPropertyPointer(SchemaItem* parent, const std::string& type, const std::string& name)
: SchemaProperty(parent, name)
, _Type(type)
{
    
}

SchemaPropertyPointer::~SchemaPropertyPointer()
{
    
}

SchemaProperty::SchemaPropertyType SchemaPropertyPointer::GetPropertyType() const
{
    return kSchemaPropertyPointer;
}

const SchemaEntity* SchemaPropertyPointer::GetSchemaEntity() const
{
    const Schema* schema = GetSchema();
    return schema->GetEntityByName(_Type);
}
