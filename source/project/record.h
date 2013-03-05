#pragma once

#include <map>
#include <string>
#include <vector>

#include "sigslot/signal.h"

#include "project/definitions.h"
#include "project/struct.h"

class Project;
class ProjectEntity;
class Schema;
class SchemaRecord;

class ProjectRecord : public ProjectStruct
{
public:
    ProjectRecord(Project* project);
    ProjectRecord(Project* project, const SchemaRecord* type, const std::string& name);
    virtual ~ProjectRecord();
    
    bool Open(const std::string& filename);
    bool Close();
    bool Save();
    bool ExportJson();
    bool ExportLua(bool exportByteCode=false);
    
    bool ExportJson(json::Object& object);
    bool ExportLua(std::iostream& output);
    
    virtual ProjectRecord* GetRecord();
    virtual const ProjectRecord* GetRecord() const;
    
public:
    typedef std::map<Uid, ProjectEntity*> EntityMap;
    
    std::string GetUniqueEntityName(const std::string& prefix);
    
    bool AddEntity(ProjectEntity* entity);
    bool RemoveEntity(ProjectEntity* entity, bool erase=true);
    
    ProjectEntity* GetEntity(Uid uid) const;
    ProjectEntity* GetEntityByName(const std::string& name);
    const EntityMap& GetEntities() const;
    
public:
    sigslot::Signal2<ProjectRecord*, ProjectEntity*> entityAdded;
    sigslot::Signal2<ProjectRecord*, ProjectEntity*> entityRemoved;
    
private:
    bool _IsOpen;
    
    EntityMap _Entities;
};
