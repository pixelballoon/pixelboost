#include <fstream>
#include <sys/stat.h>

#include "project/entity.h"
#include "project/record.h"
#include "project/project.h"
#include "project/schema.h"

#include "pixelboost/data/json/reader.h"
#include "pixelboost/data/json/writer.h"
#include "pixelboost/external/lua/lua.hpp"

extern "C" {
#include "pixelboost/external/lua/lapi.h"
#include "pixelboost/external/lua/lobject.h"
#include "pixelboost/external/lua/lundump.h"
}

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
    
    char location[1024];
    sprintf(location, "%s%X.txt", outputDir.c_str(), GetUid());

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
    
    std::fstream file(location, std::fstream::out | std::fstream::trunc);
    json::Writer::Write(record, file);
    
    file.close();
    
    return status;
}

bool Record::ExportJson()
{
    // TODO: Make OS independant
    char cmd[2048];
    std::string outputDir = GetProject()->GetConfig().exportDir + "records/";
    sprintf(cmd, "mkdir -p %s", outputDir.c_str());
    system(cmd);
    
    char location[1024];
    sprintf(location, "%s%X.pbr", outputDir.c_str(), GetUid());
    
    json::Object record;
    
    bool status = ExportJson(record);
    
    std::fstream file(location, std::fstream::out | std::fstream::trunc);
    json::Writer::Write(record, file);
    
    file.close();
    
    return status;
}

static int writeLua(lua_State*, const void* p, size_t size, void* u)
{
    return (fwrite(p,size,1,(FILE*)u)!=1) && (size!=0);
}

bool Record::ExportLua()
{
    // TODO: Make OS independant
    char cmd[2048];
    std::string outputDir = GetProject()->GetConfig().exportDir + "records/";
    sprintf(cmd, "mkdir -p %s", outputDir.c_str());
    system(cmd);

    char location[1024];
    sprintf(location, "%s%X.lua", outputDir.c_str(), GetUid());
    
    std::fstream file(location, std::fstream::out | std::fstream::trunc);
    
    bool status = ExportLua(file);
    
    file.close();
    
    // Export byte-code
    lua_State* state = luaL_newstate();
    luaL_openlibs(state);
    luaL_loadfile(state, location);
    sprintf(location, "%sc", location);
    FILE* compiledFile = fopen(location, "wb");
    TValue *o;
    lua_lock(state);
    api_checknelems(state, 1);
    o = state->top - 1;
    if (isLfunction(o))
        luaU_dump(state, getproto(o), writeLua, compiledFile, true);
    lua_unlock(state);
    fclose(compiledFile);
    lua_close(state);
    
    return status;
}

bool Record::ExportJson(json::Object& record)
{
    bool status = Struct::ExportJson(record);
    
    json::Array entities;
    
    for (EntityMap::iterator it = _Entities.begin(); it != _Entities.end(); ++it)
    {
        json::Object entity;
        if (!it->second->ExportJson(entity))
            status = false;
        
        entities.Insert(entity);
    }
    
    record["Entities"] = entities;
    
    return status;
}

bool Record::ExportLua(std::iostream& output)
{
    char record[64];
    sprintf(record, "r%X", GetUid());
    
    output << record << " = {" << std::endl;
    bool status = Struct::ExportLua(output);
    output << "}" << std::endl << std::endl;
    
    output << record << ".entities = {}" << std::endl << std::endl;
    
    int i=0;
    for (EntityMap::iterator it = _Entities.begin(); it != _Entities.end(); ++it)
    {
        output << record << ".entities[" << (++i) << "] = {" << std::endl;
        if (!it->second->ExportLua(output))
            status = false;
        output << std::endl << "}" << std::endl << std::endl;
    }
        
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
