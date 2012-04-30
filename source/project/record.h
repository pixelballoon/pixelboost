#pragma once

#include <map>
#include <string>
#include <vector>

#include "pixelboost/misc/delegate/signal.h"

#include "project/definitions.h"
#include "project/struct.h"

namespace pixeleditor
{

class Entity;
class Project;
class Schema;
class SchemaRecord;

class Record : public Struct
{
public:
    Record(Project* project);
    Record(Project* project, const SchemaRecord* type, const std::string& name);
    virtual ~Record();
    
    bool Open(const std::string& filename);
    bool Close();
    bool Save();
    bool ExportJson();
    bool ExportLua();
    
    bool ExportJson(json::Object& object);
    bool ExportLua(std::iostream& output);
    
    virtual Record* GetRecord();
    virtual const Record* GetRecord() const;
    
public:
    typedef std::map<Uid, Entity*> EntityMap;
    
    bool AddEntity(Entity* entity);
    bool RemoveEntity(Entity* entity, bool erase=true);
    
    Entity* GetEntity(Uid uid) const;
    const EntityMap& GetEntities() const;
    
public:
    sigslot::Signal2<Record*, Entity*> entityAdded;
    sigslot::Signal2<Record*, Entity*> entityRemoved;
    
private:
    bool _IsOpen;
    
    EntityMap _Entities;
};
    
}
