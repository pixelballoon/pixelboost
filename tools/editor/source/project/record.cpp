#include <fstream>
#include <sstream>
#include <sys/stat.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "json/reader.h"
#include "json/writer.h"

extern "C" {
#include "lapi.h"
#include "lobject.h"
#include "lundump.h"
}

#include "pipeline/file/fileSystem.h"
#include "pixelboost/file/fileHelpers.h"

#include "project/entity.h"
#include "project/record.h"
#include "project/project.h"
#include "project/schema.h"

ProjectRecord::ProjectRecord(Project* project)
    : ProjectStruct(project)
    , _IsOpen(false)
{
    
}

ProjectRecord::ProjectRecord(Project* project, const SchemaRecord* type, const std::string& name)
    : ProjectStruct(project, type)
    , _IsOpen(true)
{
    SetName(name);
}

ProjectRecord::~ProjectRecord()
{
    if (_IsOpen)
        Close();
}
    
bool ProjectRecord::Open(const std::string& filename)
{
    auto file = pb::FileSystem::Instance()->OpenFile(filename);
    std::string contents;
    
    if (file)
    {
        file->ReadAll(contents);
    }
    
    json::Object record;
    
    json::Reader::Read(record, contents);
    
    bool status = ProjectStruct::Open(record);
    
    // Entities
    json::Array& entities = record["Entities"];

    for (json::Array::iterator it = entities.Begin(); it != entities.End(); ++it)
    {
        json::Object& entity = *it;
        
        ProjectEntity* newEntity = new ProjectEntity(this);
        
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
    
bool ProjectRecord::Close()
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

bool ProjectRecord::Save()
{
    //pl::FileSystem::Instance()->CreateDirectory("records");
    
    char location[1024];
    sprintf(location, "records/%X.txt", GetUid());

    json::Object record;
    
    bool status = ProjectStruct::Save(record);
    
    json::Array entities;
    
    for (EntityMap::iterator it = _Entities.begin(); it != _Entities.end(); ++it)
    {
        json::Object entity;
        if (!it->second->Save(entity))
            status = false;
        
        entities.Insert(entity);
    }
    
    record["Entities"] = entities;
    
    std::stringstream contents;
    json::Writer::Write(record, contents);
    
    pl::File* file = pl::FileSystem::Instance()->OpenFile(GetProject()->GetConfig().databaseRoot + location, pl::kFileModeWrite);
    file->Write(contents.str());
    delete file;
    
    return status;
}

bool ProjectRecord::ExportJson()
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

bool ProjectRecord::ExportLua(bool exportByteCode)
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
    
    if (exportByteCode)
    {
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
    }
    
    return status;
}

bool ProjectRecord::ExportJson(json::Object& record)
{
    bool status = ProjectStruct::ExportJson(record);
    
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

bool ProjectRecord::ExportLua(std::iostream& output)
{
    char record[64];
    sprintf(record, "r%X", GetUid());
    
    output << record << " = {" << std::endl;
    bool status = ProjectStruct::ExportLua(output);
    output << "}" << std::endl << std::endl;
    
    output << record << ".entities = {" << std::endl;
    
    bool appendComma = false;
    for (EntityMap::iterator it = _Entities.begin(); it != _Entities.end(); ++it)
    {
        if (appendComma)
        {
            output << "," << std::endl;
        }
        
        appendComma = true;
        
        output << "{" << std::endl;
        if (!it->second->ExportLua(output))
            status = false;
        output << std::endl << "}";
    }
    
    output << "}";
        
    return status;
}
    
ProjectRecord* ProjectRecord::GetRecord()
{
    return this;
}
    
const ProjectRecord* ProjectRecord::GetRecord() const
{
    return this;
}

std::string ProjectRecord::GetUniqueEntityName(const std::string& prefix)
{
    int index = 0;
    
    char name[1024];
    do
    {
        snprintf(name, 1024, "%s_%03d", prefix.c_str(), index++);
        ProjectEntity* entity = GetEntityByName(name);
        
        if (!entity)
            return name;
    } while (true);
}

bool ProjectRecord::AddEntity(ProjectEntity* entity)
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

bool ProjectRecord::RemoveEntity(ProjectEntity* entity, bool erase)
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

ProjectEntity* ProjectRecord::GetEntity(Uid uid) const
{
    EntityMap::const_iterator it = _Entities.find(uid);
    
    if (it != _Entities.end())
        return it->second;
    
    return 0;
}

ProjectEntity* ProjectRecord::GetEntityByName(const std::string& name)
{
    for (EntityMap::const_iterator it = _Entities.begin(); it != _Entities.end(); ++it)
    {
        if (it->second->GetName() == name)
            return it->second;
    }
    
    return 0;
}

const ProjectRecord::EntityMap& ProjectRecord::GetEntities() const
{
    return _Entities;
}
