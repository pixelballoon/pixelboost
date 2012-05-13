#include "pixelboost/db/entity.h"
#include "pixelboost/db/manager.h"
#include "pixelboost/db/record.h"
#include "pixelboost/external/lua/lua.hpp"
#include "pixelboost/file/fileHelpers.h"

using namespace pixelboost;
    
DatabaseManager* DatabaseManager::_Instance = 0;

DatabaseManager::DatabaseManager()
{
    _DatabaseRoot = "";
    _State = luaL_newstate();
    luaL_openlibs(_State);
}

DatabaseManager::~DatabaseManager()
{
    lua_close(_State);
}

DatabaseManager* DatabaseManager::Instance()
{
    static DatabaseManager* instance = new DatabaseManager();
    return instance;
}
    
lua_State* DatabaseManager::GetLuaState()
{
    return _State;
}

void DatabaseManager::RegisterStruct(Uid type, CreateStruct createStruct)
{
    _StructCreate[type] = createStruct;
}
    
void DatabaseManager::OpenDatabase(const std::string& location)
{
    _DatabaseRoot = FileHelpers::GetRootPath() + location;
    
    std::string filename = _DatabaseRoot + "main.lua";
    
    if (luaL_loadfile(_State, filename.c_str()) || lua_pcall(_State, 0, 0, 0))
    {
        printf("Can't open file: %s", lua_tostring(_State, -1));
        return;
    }
    
    lua_getglobal(_State, "records");
    if (lua_istable(_State, -1))
    {
        lua_len(_State, -1);
        if (!lua_isnumber(_State, -1))
            return;
        
        int n = lua_tonumber(_State, -1);
        lua_pop(_State, 1);
        for (int i=1; i<=n; i++)
        {
            RecordDescription record;
            
            lua_rawgeti(_State, -1, i);
            if (!lua_istable(_State, -1))
                continue;
            
            lua_getfield(_State, -1, "type");
            record.Type = lua_tonumber(_State, -1);
            lua_pop(_State, 1);
            
            lua_getfield(_State, -1, "uid");
            record.Uid = lua_tonumber(_State, -1);
            lua_pop(_State, 1);
            
            lua_getfield(_State, -1, "name");
            record.Name = lua_tostring(_State, -1);
            lua_pop(_State, 1);
            
            _RecordDescriptions.push_back(record);
            lua_pop(_State, 1);
        }
    }
}

Record* DatabaseManager::OpenRecord(Uid recordId)
{
    char filename[1024];
    sprintf(filename, "%srecords/%X.lua", _DatabaseRoot.c_str(), recordId);
    
    if (luaL_loadfile(_State, filename) || lua_pcall(_State, 0, 0, 0))
    {
        printf("Can't open file: %s\n", lua_tostring(_State, -1));
        return 0;
    }
         
    char recordName[64];
    sprintf(recordName, "r%X", recordId);
    lua_getglobal(_State, recordName);
    
    lua_getfield(_State, -1, "type");
    
    if (!lua_isstring(_State, -1))
    {
        lua_pop(_State, -1);
        printf("Unknown record type");
        return 0;
    }
    
    Uid recordType = lua_tonumber(_State, -1);
    lua_pop(_State, 1);
    
    lua_getfield(_State, -1, "properties");
    
    if (!lua_istable(_State, -1))
    {
        lua_pop(_State, 1);
        return 0;
    }
    
    void* data = Create(recordType);
    
    lua_pop(_State, 1);
    
    Record* record = new Record(recordId, recordType, data);
    _Records[recordId] = record;
    
    lua_getfield(_State, -1, "entities");
    
    if (lua_istable(_State, -1))
    {
        lua_len(_State, -1);
        if (!lua_isnumber(_State, -1))
            return 0;
        
        int n = lua_tonumber(_State, -1);
        lua_pop(_State, 1);
        for (int i=1; i<=n; i++)
        {
            Uid uid;
            Uid type;
            void* data;
            
            lua_rawgeti(_State, -1, i);
            if (!lua_istable(_State, -1))
            {
                lua_pop(_State, 1);
                continue;
            }
            
            lua_getfield(_State, -1, "uid");
            
            if (!lua_isnumber(_State, -1))
            {
                lua_pop(_State, 1);
                continue;
            }
            uid = lua_tonumber(_State, -1);
            lua_pop(_State, 1);
            
            lua_getfield(_State, -1, "type");
            if (!lua_isnumber(_State, -1))
            {
                lua_pop(_State, 1);
                continue;
            }
            type = lua_tonumber(_State, -1);
            lua_pop(_State, 1);
            
            lua_getfield(_State, -1, "properties");
            
            if (!lua_istable(_State, -1))
            {
                lua_pop(_State, 1);
                continue;
            }
            
            data = Create(type);
            
            lua_pop(_State, 1);
            
            if (data)
            {
                Entity* entity = new Entity(uid, type, data);
                
                record->AddEntity(entity);
            } else {
                printf("Unable to create entity of type (%u)\n", type);
            }
            
            lua_pop(_State, 1);
        }
    }
    
    lua_pop(_State, 1);
    
    return record;
}
    
void* DatabaseManager::Create(Uid type)
{
    StructCreateMap::iterator it = _StructCreate.find(type);
    
    if (it == _StructCreate.end())
        return 0;
    
    return it->second();
}

const DatabaseManager::RecordDescriptionList& DatabaseManager::GetRecordDescriptions() const
{
    return _RecordDescriptions;
}
    
const DatabaseManager::RecordMap& DatabaseManager::GetRecords() const
{
    return _Records;
}

const Record* DatabaseManager::GetRecord(Uid uid) const
{
    RecordMap::const_iterator it = _Records.find(uid);
    
    if (it == _Records.end())
        return 0;
    
    return it->second;
}
