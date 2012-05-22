#pragma once

#include <map>
#include <string>
#include <vector>

#include "sigslot/signal.h"

#include "pixelboost/data/json/writer.h"
#include "pixelboost/math/maths.h"

#include "project/definitions.h"

namespace json
{
    class Object;
}

namespace pixeleditor
{

class Project;
class Property;
class PropertyAtom;
class PropertyArray;
class PropertyPointer;
class Record;
class SchemaProperty;
class SchemaStruct;

class Struct
{
public:
    Struct(Project* project, const SchemaStruct* type = 0);
    virtual ~Struct();
    
    bool Open(json::Object& object);
    bool Save(json::Object& object);
    
    bool ExportJson(json::Object& object);
    bool ExportLua(std::iostream& output, bool appendNewLine=true);
    
public:    
    Project* GetProject();
    virtual Record* GetRecord() = 0;
    virtual const Record* GetRecord() const = 0;
    
    const std::string& GetTypeName() const;
    Uid GetTypeHash() const;
    const SchemaStruct* GetType() const;
    void SetUid(Uid uid);
    Uid GetUid() const;
    
    const std::string& GetName() const;
    void SetName(const std::string& name);
    
public:
    std::string EvaluateProperty(const std::string& path, const std::string& defaultValue = "");
    
    const Property* GetProperty(const std::string& path) const;
    PropertyAtom* AcquireAtom(const std::string& path);
    PropertyPointer* AcquirePointer(const std::string& path);
    PropertyArray* AcquireArray(const std::string& path);
    
    bool RemoveProperty(const std::string& path);
    
    sigslot::Signal1<Struct*> propertyChanged;
    
private:
    bool ParseProperties(json::Object& container, std::string path);
    json::Object& GetPropertyContainer(std::string path, json::Object& parent);
    
    Uid _Uid;
    
    std::string _TypeName;
    const SchemaStruct* _Type;
    
    std::string _Name;
    
    Project* _Project;
    
private:
    typedef std::map<std::string, Property*> PropertyMap;
    
    PropertyMap _Properties;
};
    
}
