#include "pixelboost/db/manager.h"
#include "pixelboost/db/record.h"
#include "pixelboost/external/lua/lua.hpp"
#include "pixelboost/file/fileHelpers.h"

namespace pixelboost
{
    
DatabaseManager* DatabaseManager::_Instance = 0;

DatabaseManager::DatabaseManager()
{
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

void DatabaseManager::RegisterStruct(const std::string& name, CreateStruct createStruct)
{
    _StructCreate[name] = createStruct;
}
    
void DatabaseManager::OpenDatabase(const std::string& location)
{
    std::string fileRoot = FileHelpers::GetRootPath();
    
    std::string filename = fileRoot + location + "main.lua";
    
    printf("%s", filename.c_str());
    
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
            lua_rawgeti(_State, -1, i);
            if (!lua_isstring(_State, -1))
                continue;
            
            std::string recordIdStr = lua_tostring(_State, -1);
            Uid recordId;
            
            sscanf(recordIdStr.c_str(), "%X", &recordId);
            
            _RecordIds.push_back(recordId);
            lua_pop(_State, 1);
        }
    }
}

void DatabaseManager::OpenRecord(Uid recordId)
{
    /*
    std::string recordData = pixelboost::FileHelpers::FileToString(filename);
        
    json::Object record;
    
    json::Reader::Read(record, recordData);

    json::String& type = record["Type"];
    
    Struct* s = DatabaseManager::Instance()->Create(type.Value());
    
    if (!s)
        return;
    
    if (s->GetType() != 2) // kDbRecord
    {
        delete s;
        return;
    }
    
    s->Deserialise(record, 0);
    
    _Records.push_back(static_cast<Record*>(s));
    _UidMap[s->_Uid] = static_cast<Record*>(s);
     */
    
    /*
    if (luaL_loadfile(_State, filename.c_str()) || lua_pcall(_State, 0, 0, 0))
    {
        printf("Can't open file: %s", lua_tostring(_State, -1));
    }
     
    char recordName[64];
    sprintf(recordName, "r%X", recordId);
    lua_getglobal(_State, recordName);
    lua_getfield(_State, -1, "");
    */
}
    
void* DatabaseManager::Create(const std::string& name)
{
    StructCreateMap::iterator it = _StructCreate.find(name);
    
    if (it == _StructCreate.end())
        return 0;
    
    return it->second();
}

}
