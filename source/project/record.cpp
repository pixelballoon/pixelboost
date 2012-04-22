#include <fstream>
#include <sys/stat.h>

#include "project/entity.h"
#include "project/record.h"
#include "project/project.h"
#include "project/schema.h"

#include "pixelboost/data/json/reader.h"
#include "pixelboost/data/json/writer.h"
#include "pixelboost/file/fileHelpers.h"

using namespace pixeleditor;

Record::Record(Project* project)
    : Struct(project)
    , _IsOpen(false)
{
    
}

Record::Record(Project* project, const SchemaRecord* type, const std::string& name)
    : Struct(project, type)
    , _IsOpen(true)
{
    SetName(name);
}

Record::~Record()
{
    if (_IsOpen)
        Close();
}
    
bool Record::Open(const std::string& filename)
{
    std::string file = pixelboost::FileHelpers::FileToString(filename.c_str());
    
    json::Object record;
    
    json::Reader::Read(record, file);
    
    bool status = Struct::Open(record);
    
    // Entities
    json::Array& entities = record["Entities"];

    for (json::Array::iterator it = entities.Begin(); it != entities.End(); ++it)
    {
        json::Object& entity = *it;
        
        Entity* newEntity = new Entity(this);
        
        newEntity->Open(entity);
        
        if (!AddEntity(newEntity))
        {
            status = false;
            delete newEntity;
        }
    }
    
    _IsOpen = true;
    
    return status;
}
    
bool Record::Close()
{
    if (!_IsOpen)
        return false;
    
    EntityMap::iterator it = _Entities.begin();
    while (it != _Entities.end())
    {
        RemoveEntity(it->second);
        it = _Entities.begin();
    }
    
    _IsOpen = false;
    
    return true;
}

bool Record::Save()
{
    // TODO: Make OS independant
    char cmd[2048];
    std::string outputDir = GetProject()->GetLocation() + "records/";
    sprintf(cmd, "mkdir -p %s", outputDir.c_str());
    system(cmd);
    
    std::string location = outputDir + GetName() + ".txt";

    json::Object record;
    
    bool status = Struct::Save(record);
    
    json::Array entities;
    
    for (EntityMap::iterator it = _Entities.begin(); it != _Entities.end(); ++it)
    {
        json::Object entity;
        if (!it->second->Save(entity))
            status = false;
        
        entities.Insert(entity);
    }
    
    record["Entities"] = entities;
    
    std::fstream file(location.c_str(), std::fstream::out | std::fstream::trunc);
    json::Writer::Write(record, file);
    
    file.close();
    
    return status;
}
    
bool Record::Export()
{
    // TODO: Make OS independant
    char cmd[2048];
    std::string outputDir = GetProject()->GetConfig().exportDir + "records/";
    sprintf(cmd, "mkdir -p %s", outputDir.c_str());
    system(cmd);
    
    std::string location = outputDir + GetName() + ".pbr";
    
    json::Object record;
    
    bool status = Struct::Export(record);
    
    json::Array entities;
    
    for (EntityMap::iterator it = _Entities.begin(); it != _Entities.end(); ++it)
    {
        json::Object entity;
        if (!it->second->Export(entity))
            status = false;
        
        entities.Insert(entity);
    }
    
    record["Entities"] = entities;
    
    std::fstream file(location.c_str(), std::fstream::out | std::fstream::trunc);
    json::Writer::Write(record, file);
    
    file.close();
    
    return status;
}
    
Record* Record::GetRecord()
{
    return this;
}
    
const Record* Record::GetRecord() const
{
    return this;
}

bool Record::AddEntity(Entity* entity)
{
    Uid uid = entity->GetUid();
    
    EntityMap::iterator it = _Entities.find(uid);
    
    if (it == _Entities.end())
    {
        _Entities[uid] = entity;
        
        entityAdded(this, entity);
        
        return true;
    }
    
    return false;        
}

bool Record::RemoveEntity(Entity* entity, bool erase)
{
    EntityMap::iterator it = _Entities.find(entity->GetUid());
    
    if (it != _Entities.end())
    {
        _Entities.erase(it);
        
        entityRemoved(this, entity);
        
        if (erase)
            delete entity;
        
        return true;
    }
    
    return false;
}

Entity* Record::GetEntity(Uid uid) const
{
    EntityMap::const_iterator it = _Entities.find(uid);
    
    if (it != _Entities.end())
        return it->second;
    
    return 0;
}

const Record::EntityMap& Record::GetEntities() const
{
    return _Entities;
}
