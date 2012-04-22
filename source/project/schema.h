#pragma once

#include <map>
#include <string>
#include <vector>

#include "pixelboost/misc/delegate/signal.h"

#include "project/definitions.h"

namespace pixeleditor
{

class Schema;
class SchemaEntity;
class SchemaProperty;
class SchemaRecord;
class SchemaStruct;
    
class SchemaAttribute
{
public:
    SchemaAttribute(const std::string& name);
    
    const std::string& GetName() const;
    
public:
    void AddParam(const std::string& param, const std::string& value);
    std::string GetParamValue(const std::string& param, const std::string& defaultValue = "") const;
    
private:
    typedef std::map<std::string, std::string> ParamValueMap;
    
    ParamValueMap _ParamValue;
    
    std::string _Name;
};

class SchemaItem
{
public:
    SchemaItem(SchemaItem* parent, const std::string& name);
    virtual ~SchemaItem();
    
    const Schema* GetSchema() const;
    SchemaItem* GetParent();
    
    const std::string& GetName() const;
    Uid GetUid();
    
public:
    enum SchemaType
    {
        kSchema,
        kSchemaRecord,
        kSchemaEntity,
        kSchemaProperty,
        kSchemaStruct
    };
    
    virtual SchemaType GetSchemaType() const = 0;
    
public:
    void SetAttributes(const std::vector<SchemaAttribute*>& attributes);
    const SchemaAttribute* GetAttribute(const std::string& name) const;
    
private:
    typedef std::map<std::string, SchemaAttribute*> AttributeMap;
    
    AttributeMap _Attributes;
    
    SchemaItem* _Parent;
    
    std::string _Name;
    Uid _Uid;
};

class Schema : public SchemaItem
{
public:
    Schema();
    virtual ~Schema();
    
    virtual SchemaType GetSchemaType() const;
    
    bool Open(const std::string& directory);
    bool Close();
    
    Uid FindFreeUid(SchemaItem* item);
    
    bool AddStruct(SchemaStruct* schemaStruct);
    bool AddRecord(SchemaRecord* schemaRecord);
    bool AddEntity(SchemaEntity* schemaEntity);
    
public:
    typedef std::map<Uid, SchemaItem*> UidMap;
    typedef std::map<std::string, SchemaEntity*> EntityMap;
    typedef std::map<std::string, SchemaRecord*> RecordMap;
    typedef std::map<std::string, SchemaStruct*> StructMap;
    
public:
    SchemaItem* GetItemByUid(Uid uid);
    const SchemaStruct* GetStructByName(const std::string& name) const;
    const SchemaRecord* GetRecordByName(const std::string& name) const;
    const SchemaEntity* GetEntityByName(const std::string& name) const;

    const StructMap& GetStructs() const;
    const RecordMap& GetRecords() const;
    const EntityMap& GetEntities() const;
    
    sigslot::Signal1<Schema*> schemaOpened;
    sigslot::Signal1<Schema*> schemaClosed;
    
private:
    bool _IsOpen;
    UidMap _UidMap;
    
    EntityMap _Entities;
    RecordMap _Records;
    StructMap _Structs;
};
    
class SchemaStruct : public SchemaItem
{
public:
    SchemaStruct(Schema* schema, const std::string& name);
    virtual ~SchemaStruct();
    
    virtual SchemaType GetSchemaType() const;
    
    void SetBaseType(const std::string& base);
    const SchemaStruct* GetBaseType() const;
    
    void AddProperty(SchemaProperty* property);
    
public:
    typedef std::map<std::string, SchemaProperty*> PropertyMap;
    
public:
    const PropertyMap& GetProperties() const;
    const SchemaProperty* GetPropertyByName(const std::string& name) const;
    const SchemaProperty* FindPropertyByPath(const std::string& path) const;
    
private:
    PropertyMap _Properties;
    
    std::string _Base;
    mutable const SchemaStruct* _BaseCache;
};
    
class SchemaRecord : public SchemaStruct
{
public:
    SchemaRecord(Schema* schema, const std::string& name);
    virtual ~SchemaRecord();
    
    virtual SchemaType GetSchemaType() const;
};

class SchemaEntity : public SchemaStruct
{
public:
    SchemaEntity(Schema* schema, const std::string& name);
    virtual ~SchemaEntity();
    
    virtual SchemaType GetSchemaType() const;
};

class SchemaProperty : public SchemaItem
{
public:
    SchemaProperty(SchemaItem* parent, const std::string& name);
    virtual ~SchemaProperty();
    
public:
    virtual SchemaType GetSchemaType() const;
    
    enum SchemaPropertyType
    {
        kSchemaPropertyAtom,
        kSchemaPropertyArray,
        kSchemaPropertyStruct,
        kSchemaPropertyPointer,
    };
    
    virtual SchemaPropertyType GetPropertyType() const = 0;
};

class SchemaPropertyAtom : public SchemaProperty
{
public:
    SchemaPropertyAtom(SchemaItem* parent, const std::string& type, const std::string& name);
    virtual ~SchemaPropertyAtom();
    
    virtual SchemaPropertyType GetPropertyType() const;
    
public:
    enum SchemaAtomType
    {
        kSchemaAtomFloat,
        kSchemaAtomInt,
        kSchemaAtomString,
    };
    
    SchemaAtomType GetAtomType() const;
    
private:
    SchemaAtomType _AtomType;
};

class SchemaPropertyStruct : public SchemaProperty
{
public:
    SchemaPropertyStruct(SchemaItem* parent, const std::string& type, const std::string& name);
    virtual ~SchemaPropertyStruct();
    
    virtual SchemaPropertyType GetPropertyType() const;
    
    const SchemaStruct* GetSchemaStruct() const;
    
private:
    std::string _Type;
};
    
class SchemaPropertyArray : public SchemaProperty
{
public:
    SchemaPropertyArray(SchemaItem* parent, const std::string& type, const std::string& name, SchemaProperty* schemaProperty);
    virtual ~SchemaPropertyArray();
    
    virtual SchemaPropertyType GetPropertyType() const;
    
public:
    const SchemaProperty* GetSchemaProperty() const;
    
private:
    SchemaProperty* _Property;
};
    
class SchemaPropertyPointer : public SchemaProperty
{
public:
    SchemaPropertyPointer(SchemaItem* parent, const std::string& type, const std::string& name);
    virtual ~SchemaPropertyPointer();
    
    virtual SchemaPropertyType GetPropertyType() const;
    
    const SchemaEntity* GetSchemaEntity() const;
    
private:
    std::string _Type;
};
    
}
