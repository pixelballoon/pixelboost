#pragma once

#include <map>
#include <string>
#include <vector>

#include "sigslot/signal.h"

#include "pixelboost/data/json/writer.h"

#include "project/definitions.h"

namespace json
{
    class Object;
}

namespace pixeleditor
{

class Project;
class ProjectRecord;
class Property;
class PropertyAtom;
class PropertyArray;
class PropertyPointer;
class PropertyReference;
class SchemaProperty;
class SchemaStruct;

class ProjectStruct
{
public:
    ProjectStruct(Project* project, const SchemaStruct* type = 0);
    virtual ~ProjectStruct();
    
    bool Open(json::Object& object, bool skipUid = false);
    bool Save(json::Object& object);
    
    bool ExportJson(json::Object& object);
    bool ExportLua(std::iostream& output, bool appendNewLine=true);
    
public:    
    Project* GetProject();
    virtual ProjectRecord* GetRecord() = 0;
    virtual const ProjectRecord* GetRecord() const = 0;
    
    const std::string& GetTypeName() const;
    Uid GetTypeHash() const;
    const SchemaStruct* GetType() const;
    void SetUid(Uid uid);
    Uid GetUid() const;
    
    const std::string& GetName() const;
    void SetName(const std::string& name);
    
public:
    float GetPropertyFloat(const std::string& path, float defaultValue = 0.f);
    int GetPropertyInt(const std::string& path, int defaultValue = 0);
    std::string GetPropertyString(const std::string& path, const std::string& defaultValue = "");
    
    const Property* GetProperty(const std::string& path) const;
    PropertyAtom* AcquireAtom(const std::string& path);
    PropertyPointer* AcquirePointer(const std::string& path);
    PropertyReference* AcquireReference(const std::string& path);
    PropertyArray* AcquireArray(const std::string& path);
    
    bool RemoveProperty(const std::string& path);
    
    sigslot::Signal2<ProjectStruct*, Property*> propertyChanged;
    sigslot::Signal1<ProjectStruct*> destroyed;
    
private:
	void ExportJsonProperties(json::Object& object, const SchemaStruct* type);
	void ExportLuaProperties(std::iostream& output, const SchemaStruct* type, bool appendComma);
	
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
